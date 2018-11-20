// Copyright (C) 2018 Vincent Chambrin
// This file is part of the liblayout project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "example-paragraph.h"

#include "textpainter.h"

#include "tex/linebreaks.h"
#include "tex/penalty.h"

#include <QHBoxLayout>
#include <QPaintEvent>
#include <QPlainTextEdit>
#include <QSpinBox>
#include <QVBoxLayout>

class TextProcessor
{
public:
  QFont font_;
  float xHeight;
  float em;
  std::shared_ptr<tex::Glue> spaceglue;
  std::shared_ptr<tex::Glue> commaglue;
  std::shared_ptr<tex::Glue> periodglue;

  TextProcessor(const QFont & f)
    : font_(f)
  {
    QFontMetricsF metrics{ f };
    xHeight = metrics.xHeight();
    em = metrics.width(QChar('M'));
    const float space = metrics.width(QChar(' '));

    spaceglue = std::make_shared<tex::Glue>(space, 0.25f * space, 1.f * space);
    commaglue = std::make_shared<tex::Glue>(space, 0.25f * space, 1.1f * space);
    periodglue = std::make_shared<tex::Glue>(space, 0.25f * space, 1.25f * space);
  }

  tex::List process(const QString & text);

  static QString readToken(const QString & text, int & pos);
};

QString TextProcessor::readToken(const QString & text, int & pos)
{
  if (pos == text.size())
    return QString();

  while (pos != text.size() && text.at(pos).isSpace())
    ++pos;

  if (pos == text.size())
    return QString();

  if (text.at(pos).isPunct())
  {
    const int start = pos;
    while (pos < text.size() && text.at(pos).isPunct())
      ++pos;

    return text.mid(start, pos - start);
  }
  else
  {
    const int start = pos;
    while (pos < text.size() && !text.at(pos).isPunct() && !text.at(pos).isSpace())
      ++pos;

    return text.mid(start, pos - start);
  }
}

tex::List TextProcessor::process(const QString & text)
{
  tex::List result;

  int pos = 0;
  QString w = readToken(text, pos);
  bool insert_space = false;

  while (!w.isNull())
  {
    if (w.size() == 1 && w.front().isPunct())
    {
      insert_space = false;
      result.push_back(stringbox(w, font_));
      if (w.front() == ',')
        result.push_back(commaglue);
      else if (w.front() == '.')
        result.push_back(periodglue);
      else
        result.push_back(spaceglue);
    }
    else
    {
      if (insert_space)
        result.push_back(spaceglue);

      result.push_back(stringbox(w, font_));
      insert_space = true;
    }

    w = readToken(text, pos);
  }

  return result;
}

ParagraphTextWidget::ParagraphTextWidget(QWidget *parent)
  : Example(parent)
{
  text_ = new QPlainTextEdit;
  text_->setPlainText("A long time ago in a galaxy far, far away...");

  connect(text_, SIGNAL(textChanged()), this, SLOT(recomputeLayout()));

  recomputeLayout(800);
}

QString ParagraphTextWidget::getName() const
{
  return "Paragraph";
}

QWidget* ParagraphTextWidget::getController()
{
  return text_;
}

void ParagraphTextWidget::recomputeLayout(int pagewidth)
{
  if (text_->toPlainText().isEmpty())
    return;

  if (pagewidth == -1)
    pagewidth = pagewidth_;

  QFont font = QFont{ "Times New Roman" };
  font.setPointSize(36);

  TextProcessor tp{ font };
  tex::List l = tp.process(text_->toPlainText());

  tex::Paragraph paragraph;
  paragraph.parshape() = { float(pagewidth) };
  //paragraph.leftskip() = std::make_shared<tex::Glue>(0.f, 0.f, 1.f, tex::GlueOrder::Normal, tex::GlueOrder::Fill);
  //paragraph.rightskip() = paragraph.leftskip();

  paragraph.prepare(l);
  l = paragraph.create(l);

  layout_ = tex::vbox(std::move(l));
  pagewidth_ = pagewidth;

  update();
}

void ParagraphTextWidget::paintEvent(QPaintEvent *e)
{
  QPainter p{ this };

  TextPainter painter{ &p };

  const float x = (width() - pagewidth_) * 0.5f;
  const float y = (height() - layout_->totalHeight()) * 0.5f;

  tex::read(painter, layout_, { x, y });
}
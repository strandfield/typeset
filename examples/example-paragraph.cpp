// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "example-paragraph.h"

#include "textpainter.h"
#include "textprocessor.h"

#include "tex/linebreaks.h"
#include "tex/penalty.h"

#include <QHBoxLayout>
#include <QPaintEvent>
#include <QPlainTextEdit>
#include <QSpinBox>
#include <QVBoxLayout>

ParagraphTextWidget::ParagraphTextWidget(QWidget *parent)
  : Example(parent)
{
  text_ = new QPlainTextEdit;
  text_->setPlainText("A long time ago in a galaxy far, far away...");

  connect(text_, SIGNAL(textChanged()), this, SLOT(recomputeLayout()));

  recomputeLayout(600);
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
  font.setPointSize(18);

  TextProcessor tp{ font };
  tex::List l = tp.process(text_->toPlainText());

  tex::Paragraph paragraph;
  paragraph.hsize = pagewidth;
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
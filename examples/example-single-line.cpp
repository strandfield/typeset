// Copyright (C) 2018 Vincent Chambrin
// This file is part of the liblayout project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "example-single-line.h"

#include "textpainter.h"

#include <QPaintEvent>
#include <QSpinBox>

SingleLineTextWidget::SingleLineTextWidget(QWidget *parent)
  : Example(parent)
{
  recomputeLayout(-1);
}

QString SingleLineTextWidget::getName() const
{
  return "The senate will decide your fate";
}

QWidget* SingleLineTextWidget::getController()
{
  QSpinBox *spinbox = new QSpinBox;
  spinbox->setRange(10, 1200);
  spinbox->setValue(pagewidth_);
  connect(spinbox, SIGNAL(valueChanged(int)), this, SLOT(recomputeLayout(int)));
  return spinbox;
}

void SingleLineTextWidget::recomputeLayout(int pagewidth)
{
  QStringList words = QString("I am the senate").split(' ');

  QFont font = QFont{ "Times New Roman" };
  font.setPointSize(48);

  QFontMetricsF metrics{ font };
  const float xHeight = metrics.xHeight();
  const float em = metrics.width(QChar('M'));
  const float space = metrics.width(QChar(' '));

  auto spacenode = std::make_shared<tex::Glue>(space, 0.25f * space, 1.f);

  tex::List list;
  for (const auto & w : words)
  {
    list.push_back(stringbox(w, font));
    list.push_back(spacenode);
  }

  list.pop_back();
  list.push_back(stringbox(".", font));

  if (pagewidth == -1)
  {
    layout_ = tex::hbox(std::move(list));
    pagewidth_ = layout_->width();
  }
  else
  {
    layout_ = tex::hbox(std::move(list), pagewidth);
    pagewidth_ = pagewidth;
  }

  update();
}

void SingleLineTextWidget::paintEvent(QPaintEvent *e)
{
  QPainter p{ this };

  TextPainter painter{ &p };

  const float x = (width() - pagewidth_) * 0.5f;
  const float y = (height() - layout_->totalHeight()) * 0.5f;

  tex::read(painter, layout_, { x, y });
}
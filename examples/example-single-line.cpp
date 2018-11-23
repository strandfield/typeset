// Copyright (C) 2018 Vincent Chambrin
// This file is part of the liblayout project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "example-single-line.h"

#include "textpainter.h"
#include "textprocessor.h"

#include <QPaintEvent>
#include <QSpinBox>

SingleLineTextWidget::SingleLineTextWidget(QWidget *parent)
  : Example(parent)
{
  recomputeLayout(-1);
}

QString SingleLineTextWidget::getName() const
{
  return "A New Hope";
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
  QString input = "Help me, Obi-Wan Kenobi. You're my only hope.";

  QFont font = QFont{ "Times New Roman" };
  font.setPointSize(26);

  TextProcessor tp{ font };

  tex::List list = tp.process(input);

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
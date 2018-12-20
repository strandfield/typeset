// Copyright (C) 2018 Vincent Chambrin
// This file is part of the liblayout project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "example-single-line.h"

#include "textpainter.h"
#include "textprocessor.h"

#include <QPaintEvent>
#include <QSpinBox>

#include <QDebug>

SingleLineTextWidget::SingleLineTextWidget(QWidget *parent)
  : Example(parent)
{
  constructLayout();
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

void SingleLineTextWidget::constructLayout()
{
  QString input = "Help me, Obi-Wan Kenobi. You're my only hope.";

  QFont font = QFont{ "Times New Roman" };
  font.setPointSize(26);

  TextProcessor tp{ font };

  tex::List list = tp.process(input);

  layout_ = tex::hbox(std::move(list));
  pagewidth_ = layout_->width();
}

void SingleLineTextWidget::recomputeLayout(int pagewidth)
{
  tex::HBoxEditor editor{ *layout_ };
  if (pagewidth < 0)
  {
    editor.rebox();
    pagewidth_ = layout_->width();
  }
  else
  {
    auto result = editor.rebox(pagewidth);
    if (result == tex::BoxingResult::OverfullBox)
      qDebug() << "Overfull box : layoutwidth =" << layout_->width() << ", pagewidth=" << pagewidth;
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
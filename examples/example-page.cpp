// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "example-page.h"

#include "textpainter.h"
#include "textprocessor.h"
#include "typeset-qt.h"

#include "tex/linebreaks.h"
#include "tex/penalty.h"
#include "tex/parsing/typesetting-machine.h"
#include "tex/parsing/verticalmode.h"

#include <QHBoxLayout>
#include <QPaintEvent>
#include <QPlainTextEdit>
#include <QSpinBox>
#include <QVBoxLayout>

PageTextWidget::PageTextWidget(QWidget *parent)
  : Example(parent)
{
  typeset_engine_ = std::make_shared<QtTypesetEngine>();

  text_ = new QPlainTextEdit;
  text_->setPlainText("A long time ago in a galaxy far, far away...");

  connect(text_, SIGNAL(textChanged()), this, SLOT(recomputeLayout()));

  recomputeLayout(600);
}

QString PageTextWidget::getName() const
{
  return "Page";
}

QWidget* PageTextWidget::getController()
{
  return text_;
}

void PageTextWidget::recomputeLayout(int pagewidth)
{
  if (text_->toPlainText().isEmpty())
    return;

  if (pagewidth == -1)
    pagewidth = pagewidth_;

  tex::parsing::TypesettingMachine machine{ typeset_engine_, tex::Options{typeset_engine_} };
  auto* vm = static_cast<tex::parsing::VerticalMode*>(machine.modes().front().get());
  vm->setPageWidth(pagewidth);

  machine.inputStream().push_back(tex::parsing::InputStream::Document::fromString(text_->toPlainText().toStdString()));

  while (!machine.inputStream().atEnd())
  {
    machine.advance();
  }

  tex::List vlist = vm->vlist();

  layout_ = tex::vbox(std::move(vlist));
  pagewidth_ = pagewidth;

  update();
}

void PageTextWidget::paintEvent(QPaintEvent *e)
{
  QPainter p{ this };

  TextPainter painter{ &p };

  const float x = (width() - pagewidth_) * 0.5f;
  const float y = (height() - layout_->totalHeight()) * 0.5f;

  tex::read(painter, layout_, { x, y });
}
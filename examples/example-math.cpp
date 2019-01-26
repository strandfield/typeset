// Copyright (C) 2018 Vincent Chambrin
// This file is part of the liblayout project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "example-math.h"

#include "textpainter.h"
#include "typeset-qt.h"

#include "tex/math/atom.h"
#include "tex/math/fraction.h"
#include "tex/math/math-typeset.h"

#include <QPaintEvent>

static std::shared_ptr<tex::Symbol> sym(char c)
{
  return std::make_shared<QCharSymbol>(c);
}

static std::shared_ptr<tex::Symbol> sym(QChar c)
{
  return std::make_shared<QCharSymbol>(c);
}

static std::shared_ptr<tex::math::Atom> ord(char c)
{
  return tex::math::Atom::create<tex::math::Atom::Ord>(sym(c));
}

static std::shared_ptr<tex::math::Atom> op(QChar c)
{
  return tex::math::Atom::create<tex::math::Atom::Op>(sym(c));
}

static std::shared_ptr<tex::math::Fraction> frac(tex::MathList && num, tex::MathList && den)
{
  return std::make_shared<tex::math::Fraction>(std::move(num), std::move(den));
}

static std::shared_ptr<tex::math::Fraction> frac(std::shared_ptr<tex::Node> num, std::shared_ptr<tex::Node> den)
{
  tex::MathList numer;
  numer.push_back(num);
  tex::MathList denom;
  denom.push_back(den);

  return std::make_shared<tex::math::Fraction>(std::move(numer), std::move(denom));
}

MathTextWidget::MathTextWidget(QWidget *parent)
  : Example(parent)
{
  tex::MathList mlist;
  mlist.push_back(ord('2'));
  mlist.push_back(op(QChar(0x00D7)));
  mlist.push_back(frac(ord('x'), ord('7')));

  auto engine = std::make_shared<QtTypesetEngine>();

  tex::Options opts{ engine };
  tex::List hlist = tex::mlist_to_hlist(std::move(mlist), opts);
  layout_ = tex::hbox(std::move(hlist));
}

QString MathTextWidget::getName() const
{
  return "Math";
}

QWidget* MathTextWidget::getController()
{
  return nullptr;
}

void MathTextWidget::recomputeLayout(int pagewidth)
{
  
}

void MathTextWidget::paintEvent(QPaintEvent *e)
{
  QPainter p{ this };

  TextPainter painter{ &p };

  const float x = (width() - layout_->width()) * 0.5f;
  const float y = (height() - layout_->totalHeight()) * 0.5f;

  tex::read(painter, layout_, { x, y });
}
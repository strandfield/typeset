// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "example-math.h"

#include "textpainter.h"
#include "typeset-qt.h"

#include "tex/math/atom.h"
#include "tex/math/fraction.h"
#include "tex/math/math-typeset.h"
#include "tex/parsing/mathparserfrontend.h"

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
  tex::parsing::MathParserFrontend mathparser;

  mathparser.writeChar('2');
  mathparser.writeControlSequence("times");
  mathparser.writeControlSequence("frac");
  mathparser.beginMathList();
  mathparser.writeChar('x');
  mathparser.endMathList();
  mathparser.beginMathList();
  mathparser.writeChar('7');
  mathparser.endMathList();
  mathparser.finish();

  tex::MathList mlist = mathparser.output();
  //mlist.push_back(ord('2'));
  //mlist.push_back(op(QChar(0x00D7)));
  //mlist.push_back(frac(ord('x'), ord('7')));

  auto engine = std::make_shared<QtTypesetEngine>();

  tex::MathTypesetter mt{ engine };
  mt.setInsertPenalties(false);

  {
    std::array<tex::MathFont, 16> fonts;

    for (size_t i(0); i < 16; ++i)
    {
      fonts[i].textfont = tex::Font(3 * i + 1);
      fonts[i].scriptfont = tex::Font(3 * i + 2);
      fonts[i].scriptscriptfont = tex::Font(3 * i + 3);
    }

    mt.setFonts(fonts);
  }

  tex::List hlist = mt.mlist2hlist(mlist);
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
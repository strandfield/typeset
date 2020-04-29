// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_MATH_FRACTION_H
#define LIBTYPESET_MATH_FRACTION_H

#include "tex/math/mathlist.h"

namespace tex
{

namespace math
{

class LIBTYPESET_API Fraction : public Node
{
public:
  ~Fraction() = default;

  explicit Fraction(bool bar = true)
    : mBar(bar)
  {

  }

  Fraction(MathList && n, MathList && d, bool bar = true)
    : mNumer(std::move(n)), mDenom(std::move(d)), mBar(bar)
  {

  }

  bool isFraction() const override;

  inline const MathList & numer() const { return mNumer; }
  inline const MathList & denom() const { return mDenom; }
  inline bool hasBar() const { return mBar; }

  inline MathList& numer() { return mNumer; }
  inline MathList& denom() { return mDenom; }

private:
  MathList mNumer;
  MathList mDenom;
  bool mBar;
};

} // namespace math

} // namespace tex

#endif // LIBTYPESET_MATH_FRACTION_H

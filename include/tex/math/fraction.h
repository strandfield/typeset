// Copyright (C) 2018 Vincent Chambrin
// This file is part of the liblayout project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBLAYOUT_MATH_FRACTION_H
#define LIBLAYOUT_MATH_FRACTION_H

#include "tex/math/mathlist.h"

namespace tex
{

namespace math
{

class LIBLAYOUT_API Fraction : public Node
{
public:
  Fraction(MathList && n, MathList && d, bool bar)
    : mNumer(std::move(n)), mDenom(std::move(d)), mBar(bar)
  {

  }
  ~Fraction() = default;

  inline const MathList & numer() const { return mNumer; }
  inline const MathList & denom() const { return mDenom; }
  inline bool hasBar() const { return mBar; }

private:
  MathList mNumer;
  MathList mDenom;
  bool mBar;
};

} // namespace math

} // namespace tex

#endif // LIBLAYOUT_MATH_FRACTION_H

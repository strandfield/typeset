// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_MATH_ROOT_H
#define LIBTYPESET_MATH_ROOT_H

#include "tex/math/mathlist.h"

namespace tex
{

namespace math
{

class LIBTYPESET_API Root : public Node
{
public:
  Root(MathList && deg, MathList && rad)
    : mDegree(std::move(deg)), mRadicand(std::move(rad))
  {

  }
  ~Root() = default;

  inline const MathList & degree() const { return mDegree; }
  inline const MathList & radicand() const { return mRadicand; }

  inline MathList& degree() { return mDegree; }
  inline MathList& radicand() { return mRadicand; }

private:
  MathList mDegree;
  MathList mRadicand;
};

} // namespace math

} // namespace tex

#endif // LIBTYPESET_MATH_ROOT_H

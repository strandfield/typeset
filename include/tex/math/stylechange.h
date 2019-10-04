// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_MATH_STYLECHANGE_H
#define LIBTYPESET_MATH_STYLECHANGE_H

#include "tex/node.h"
#include "tex/math/style.h"

namespace tex
{

namespace math
{

class LIBTYPESET_API StyleChange : public Node
{
private:
  math::Style m_style;

public:
  StyleChange(math::Style s)
    : m_style(s)
  {

  }

  ~StyleChange() = default;


  math::Style style() const { return m_style; }
};

} // namespace math

} // namespace tex

#endif // LIBTYPESET_MATH_STYLECHANGE_H

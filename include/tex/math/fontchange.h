// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_MATH_FONTCHANGE_H
#define LIBTYPESET_MATH_FONTCHANGE_H

#include "tex/node.h"
#include "tex/font.h"

namespace tex
{

namespace math
{

class LIBTYPESET_API FontChange : public Node
{
private:
  Font m_font;

public:
  FontChange(Font f)
    : m_font(f)
  {

  }

  ~FontChange() = default;


  Font font() const { return m_font; }
};

} // namespace math

} // namespace tex

#endif // LIBTYPESET_MATH_FONTCHANGE_H

// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_FONT_H
#define LIBTYPESET_FONT_H

#include "tex/defs.h"

namespace tex
{

class LIBTYPESET_API Font
{
public:
  explicit Font(int val) : mFont(val) { }
  Font(const Font & ) = default;
  ~Font() = default;

  static const Font Default;
  static const Font Text;
  static const Font MathRoman;
  static const Font MathItalic;

  inline int id() const { return mFont; }

  Font & operator=(const Font & ) = default;

private:
  int mFont;
};

inline bool operator==(const Font & a, const Font & b) { return a.id() == b.id(); }
inline bool operator!=(const Font & a, const Font & b) { return b.id() != a.id(); }

} // namespace tex

#endif // LIBTYPESET_FONT_H

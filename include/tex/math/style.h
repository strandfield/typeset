// Copyright (C) 2018 Vincent Chambrin
// This file is part of the liblayout project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBLAYOUT_MATH_STYLE_H
#define LIBLAYOUT_MATH_STYLE_H

#include "tex/font.h"

namespace tex
{

namespace math
{

class LIBLAYOUT_API Style
{
public:
  Style(const Style & ) = default;
  ~Style() = default;

  Style sup() const;
  Style sub() const;

  Style fracNum() const;
  Style fracDen() const;

  Style cramp() const;

  Style text() const;

  bool isTight() const;

  static const Style D;
  static const Style Dc;
  static const Style T;
  static const Style Tc;
  static const Style S;
  static const Style Sc;
  static const Style SS;
  static const Style SSc;

  FontSize sizeForStyle(FontSize currentSize = FontSize::Normal) const;

  inline int id() const { return mId; }
  inline bool isCramped() const { return mCramped; }

  Style & operator=(const Style &) = default;

protected:
  friend struct StyleConstructor;
  Style(int, int, bool);

private:
  int mId;
  int mSize;
  bool mCramped;
};

inline bool operator==(const Style & a, const Style & b) { return a.id() == b.id(); }
inline bool operator!=(const Style & a, const Style & b) { return a.id() != b.id(); }

inline bool operator<=(const Style & a, const Style & b) { return a.id() >= b.id(); }
inline bool operator<(const Style & a, const Style & b) { return a.id() > b.id(); }
inline bool operator>(const Style & a, const Style & b) { return b < a; }
inline bool operator>=(const Style & a, const Style & b) { return b <= a; }

} // namespace math

} // namespace tex

#endif // LIBLAYOUT_MATH_STYLE_H

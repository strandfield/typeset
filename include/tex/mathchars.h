// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_MATHCHARS_H
#define LIBTYPESET_MATHCHARS_H

#include "unicode.h"

namespace tex
{

struct mathchars
{
  static constexpr Character BULLET = 0x2B24;
  static constexpr Character CAP = 0x2229;
  static constexpr Character CDOT = 0x22C5;
  static constexpr Character CIRC = 0x25EF;
  static constexpr Character CUP = 0x222A;
  static constexpr Character SQCAP = 0x2293;
  static constexpr Character SQCUP = 0x2294;
  static constexpr Character SQRT = 0x221A;
  static constexpr Character TIMES = 0x00D7;
};

} // namespace tex

#endif // LIBTYPESET_MATHCHARS_H

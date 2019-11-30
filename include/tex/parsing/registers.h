// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_REGISTERS_H
#define LIBTYPESET_REGISTERS_H

#include "tex/defs.h"

#include <array>
#include <stdexcept>
#include <string>

namespace tex
{

namespace parsing
{

struct Registers
{
  int r0 = 0;
  int r1 = 0;
  int r2 = 0;
  int r3 = 0;
  int r4 = 0;
  int r5 = 0;
  int r6 = 0;
  int r7 = 0;
  int r8 = 0;
  int r9 = 0;
  int r10 = 0;
  int r11 = 0;
  int r12 = 0;
  int r13 = 0;
  int r14 = 0;
  int r15 = 0;
  int br = 0;
};

} // namespace parsing

} // namespace tex

#endif // LIBTYPESET_REGISTERS_H

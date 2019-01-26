// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_MATH_ON_OFF_H
#define LIBTYPESET_MATH_ON_OFF_H

#include "tex/node.h"

namespace tex
{

class LIBTYPESET_API MathOn : public Node
{
public:
  MathOn() = default;
  ~MathOn() = default;
};

class LIBTYPESET_API MathOff : public Node
{
public:
  MathOff() = default;
  ~MathOff() = default;
};

} // namespace tex

#endif // LIBTYPESET_MATH_ON_OFF_H

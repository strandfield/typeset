// Copyright (C) 2018 Vincent Chambrin
// This file is part of the liblayout project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBLAYOUT_MATH_ON_OFF_H
#define LIBLAYOUT_MATH_ON_OFF_H

#include "tex/node.h"

namespace tex
{

class LIBLAYOUT_API MathOn : public Node
{
public:
  MathOn() = default;
  ~MathOn() = default;
};

class LIBLAYOUT_API MathOff : public Node
{
public:
  MathOff() = default;
  ~MathOff() = default;
};

} // namespace tex

#endif // LIBLAYOUT_MATH_ON_OFF_H

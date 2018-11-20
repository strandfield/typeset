// Copyright (C) 2018 Vincent Chambrin
// This file is part of the liblayout project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBLAYOUT_BOX_H
#define LIBLAYOUT_BOX_H

#include "tex/node.h"

namespace tex
{

class LIBLAYOUT_API Box : public Node
{
public:
  virtual float height() const = 0;
  virtual float depth() const = 0;
  virtual float width() const = 0;

  inline float totalHeight() const
  {
    return height() + depth();
  }
};

} // namespace tex

#endif // LIBLAYOUT_BOX_H

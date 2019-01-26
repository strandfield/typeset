// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_BOX_H
#define LIBTYPESET_BOX_H

#include "tex/node.h"
#include "tex/boxmetrics.h"

namespace tex
{

class LIBTYPESET_API Box : public Node
{
public:
  virtual float height() const = 0;
  virtual float depth() const = 0;
  virtual float width() const = 0;

  inline float totalHeight() const
  {
    return height() + depth();
  }

  inline BoxMetrics metrics() const { return BoxMetrics{ height(), depth(), width() }; }
};

} // namespace tex

#endif // LIBTYPESET_BOX_H

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
private:
  float m_height;
  float m_depth;
  float m_width;

public:

  Box();

  explicit Box(const BoxMetrics& metrics);
  Box(float h, float d, float w);

  bool isBox() const override;

  float height() const { return m_height; }
  float depth() const { return m_depth; }
  float width() const { return m_width; }

  float totalHeight() const
  {
    return height() + depth();
  }

  BoxMetrics metrics() const { return BoxMetrics{ height(), depth(), width() }; }

protected:
  void setHeight(float h);
  void setDepth(float d);
  void setWidth(float w);
  void reset(float h, float d, float w);
};

} // namespace tex

#endif // LIBTYPESET_BOX_H

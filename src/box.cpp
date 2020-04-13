// Copyright (C) 2019-2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/box.h"

namespace tex
{

Box::Box()
  : m_height(0.f),
  m_depth(0.f),
  m_width(0.f)
{

}

Box::Box(const BoxMetrics& metrics)
  : m_height(metrics.height),
  m_depth(metrics.depth),
  m_width(metrics.width)
{

}

Box::Box(float h, float d, float w)
  : m_height(h),
  m_depth(d),
  m_width(w)
{

}

bool Box::isBox() const
{
  return true;
}

void Box::setHeight(float h)
{
  m_height = h;
}

void Box::setDepth(float d)
{
  m_depth = d;
}

void Box::setWidth(float w)
{
  m_width = w;
}

void Box::reset(float h, float d, float w)
{
  m_height = h;
  m_depth = d;
  m_width = w;
}

} // namespace tex

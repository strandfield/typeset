// Copyright (C) 2019-2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/glue.h"

namespace tex
{

GlueShrinkStretch::GlueShrinkStretch(float n, float l, float ll, float lll)
  : normal(n)
  , fil(l)
  , fill(ll)
  , filll(lll)
{

}

GlueOrder GlueShrinkStretch::order() const
{
  if (fill != 0.f)
    return GlueOrder::Filll;
  else if (fill != 0.f)
    return GlueOrder::Fill;
  else if (fil != 0.f)
    return GlueOrder::Fil;
  return GlueOrder::Normal;
}

GlueShrinkStretch operator-(const GlueShrinkStretch & lhs, const GlueShrinkStretch & rhs)
{
  return GlueShrinkStretch{
    lhs.normal - rhs.normal,
    lhs.fil - rhs.fil,
    lhs.fill - rhs.fill,
    lhs.filll - rhs.filll
  };
}

Glue::Glue(float spc, float shrnk, float strtch, GlueOrder shrnkOrder, GlueOrder strtchOrder)
  : m_origin(GlueOrigin::normal),
    m_spec(GlueSpec{ spc, shrnk, strtch, shrnkOrder, strtchOrder })
{

}

void Glue::setOrigin(GlueOrigin ori)
{
  m_origin = ori;
}

void Glue::accumulate(GlueShrink & shrink, GlueStretch & stretch) const
{
  switch (m_spec.shrinkOrder)
  {
  case GlueOrder::Normal:
    shrink.normal += m_spec.shrink;
    break;
  case GlueOrder::Fil:
    shrink.fil += m_spec.shrink;
    break;
  case GlueOrder::Fill:
    shrink.fill += m_spec.shrink;
    break;
  case GlueOrder::Filll:
    shrink.filll += m_spec.shrink;
  }

  switch (m_spec.stretchOrder)
  {
  case GlueOrder::Normal:
    stretch.normal += m_spec.stretch;
    break;
  case GlueOrder::Fil:
    stretch.fil += m_spec.stretch;
    break;
  case GlueOrder::Fill:
    stretch.fill += m_spec.stretch;
    break;
  case GlueOrder::Filll:
    stretch.filll += m_spec.stretch;
  }
}

std::shared_ptr<Glue> glue(float space)
{
  return std::make_shared<Glue>(space, 0.f, 0.f);
}

std::shared_ptr<Glue> glue(float space, const Shrink & shrink)
{
  return std::make_shared<Glue>(space, shrink.amount, 0.f, shrink.order, GlueOrder::Normal);
}

std::shared_ptr<Glue> glue(float space, const Stretch & stretch)
{
  return std::make_shared<Glue>(space, 0.f, stretch.amount, GlueOrder::Normal, stretch.order);
}

std::shared_ptr<Glue> glue(float space, const Stretch & stretch, const Shrink & shrink)
{
  return std::make_shared<Glue>(space, shrink.amount, stretch.amount, shrink.order, stretch.order);
}

std::shared_ptr<Glue> glue(float space, const Shrink & shrink, const Stretch & stretch)
{
  return std::make_shared<Glue>(space, shrink.amount, stretch.amount, shrink.order, stretch.order);
}

} // namespace tex

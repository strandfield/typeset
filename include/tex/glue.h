// Copyright (C) 2019-2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_GLUE_H
#define LIBTYPESET_GLUE_H

#include "tex/node.h"

#include <memory>

namespace tex
{

enum class GlueOrder
{
  Normal,
  Fil,
  Fill,
  Filll,
};

enum class GlueOrigin
{
  normal,
  mskip,
  nonscript,
};

struct GlueSpec
{
  float space;
  float shrink;
  float stretch;
  GlueOrder shrinkOrder;
  GlueOrder stretchOrder;
};

struct LIBTYPESET_API GlueShrinkStretch
{
  GlueShrinkStretch(float n = 0.f, float l = 0.f, float ll = 0.f, float lll = 0.f);

  float normal;
  float fil;
  float fill;
  float filll;

  GlueOrder order() const;
};

LIBTYPESET_API GlueShrinkStretch operator-(const GlueShrinkStretch & lhs, const GlueShrinkStretch & rhs);

struct GlueShrink : GlueShrinkStretch
{
  using GlueShrinkStretch::GlueShrinkStretch;
};

struct GlueStretch : GlueShrinkStretch
{
  using GlueShrinkStretch::GlueShrinkStretch;
};

typedef GlueShrink ShrinkTotals;
typedef GlueStretch StretchTotals;

struct GlueSettings
{
  float ratio;
  GlueOrder order;
};

class LIBTYPESET_API Glue final : public Node
{
public:
  Glue(float spc, float shrnk, float strtch, GlueOrder shrnkOrder = GlueOrder::Normal, GlueOrder strtchOrder = GlueOrder::Normal);
  ~Glue() = default;

  float space() const { return m_spec.space; }
  float shrink() const { return m_spec.shrink; }
  float stretch() const { return m_spec.stretch; }
  GlueOrder shrinkOrder() const { return m_spec.shrinkOrder; }
  GlueOrder stretchOrder() const { return m_spec.stretchOrder; }

  GlueOrigin origin() const { return m_origin; }
  void setOrigin(GlueOrigin ori);

  void accumulate(GlueShrink & shrink, GlueStretch & stretch) const;

  GlueSpec& spec() { return m_spec; }
  const GlueSpec& spec() const { return m_spec; }

private:
  GlueOrigin m_origin;
  GlueSpec m_spec;
};

struct LIBTYPESET_API Shrink
{
  float amount;
  GlueOrder order;

  Shrink(float a, GlueOrder o = GlueOrder::Normal)
    : amount(a)
    , order(o)
  {

  }
};

struct LIBTYPESET_API Stretch
{
  float amount;
  GlueOrder order;

  Stretch(float a, GlueOrder o = GlueOrder::Normal)
    : amount(a)
    , order(o)
  {

  }
};

LIBTYPESET_API std::shared_ptr<Glue> glue(float space);
LIBTYPESET_API std::shared_ptr<Glue> glue(float space, const Shrink & shrink);
LIBTYPESET_API std::shared_ptr<Glue> glue(float space, const Stretch & stretch);
LIBTYPESET_API std::shared_ptr<Glue> glue(float space, const Stretch & stretch, const Shrink & shrink);
LIBTYPESET_API std::shared_ptr<Glue> glue(float space, const Shrink & shrink, const Stretch & stretch);

} // namespace tex

#endif // LIBTYPESET_GLUE_H

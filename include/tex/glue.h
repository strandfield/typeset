// Copyright (C) 2018 Vincent Chambrin
// This file is part of the liblayout project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBLAYOUT_GLUE_H
#define LIBLAYOUT_GLUE_H

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

struct GlueSpec
{
  float space;
  float shrink;
  float stretch;
  GlueOrder shrinkOrder;
  GlueOrder stretchOrder;
};

struct LIBLAYOUT_API GlueShrinkStretch
{
  GlueShrinkStretch(float n = 0.f, float l = 0.f, float ll = 0.f, float lll = 0.f);

  float normal;
  float fil;
  float fill;
  float filll;

  GlueOrder order() const;
};

LIBLAYOUT_API GlueShrinkStretch operator-(const GlueShrinkStretch & lhs, const GlueShrinkStretch & rhs);

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

class LIBLAYOUT_API Glue final : public Node
{
public:
  Glue(float spc, float shrnk, float strtch, GlueOrder shrnkOrder = GlueOrder::Normal, GlueOrder strtchOrder = GlueOrder::Normal);
  ~Glue() = default;

  inline float space() const { return mGlue.space; }
  inline float shrink() const { return mGlue.shrink; }
  inline float stretch() const { return mGlue.stretch; }
  inline GlueOrder shrinkOrder() const { return mGlue.shrinkOrder; }
  inline GlueOrder stretchOrder() const { return mGlue.stretchOrder; }

  void accumulate(GlueShrink & shrink, GlueStretch & stretch) const;

  inline const GlueSpec & spec() const { return mGlue; }

private:
  const GlueSpec mGlue;
};

struct LIBLAYOUT_API Shrink
{
  float amount;
  GlueOrder order;

  Shrink(float a, GlueOrder o = GlueOrder::Normal)
    : amount(a)
    , order(o)
  {

  }
};

struct LIBLAYOUT_API Stretch
{
  float amount;
  GlueOrder order;

  Stretch(float a, GlueOrder o = GlueOrder::Normal)
    : amount(a)
    , order(o)
  {

  }
};

LIBLAYOUT_API std::shared_ptr<Glue> glue(float space);
LIBLAYOUT_API std::shared_ptr<Glue> glue(float space, const Shrink & shrink);
LIBLAYOUT_API std::shared_ptr<Glue> glue(float space, const Stretch & stretch);
LIBLAYOUT_API std::shared_ptr<Glue> glue(float space, const Stretch & stretch, const Shrink & shrink);
LIBLAYOUT_API std::shared_ptr<Glue> glue(float space, const Shrink & shrink, const Stretch & stretch);

} // namespace tex

#endif // LIBLAYOUT_GLUE_H

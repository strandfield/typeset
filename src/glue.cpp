
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

Glue::Glue(float spc, float shrnk, float strtch, GlueOrder shrnkOrder, GlueOrder strtchOrder)
  : mGlue(GlueSpec{ spc, shrnk, strtch, shrnkOrder, strtchOrder })
{

}

void Glue::accumulate(GlueShrink & shrink, GlueStretch & stretch) const
{
  switch (mGlue.shrinkOrder)
  {
  case GlueOrder::Normal:
    shrink.normal += mGlue.shrink;
    break;
  case GlueOrder::Fil:
    shrink.fil += mGlue.shrink;
    break;
  case GlueOrder::Fill:
    shrink.fill += mGlue.shrink;
    break;
  case GlueOrder::Filll:
    shrink.filll += mGlue.shrink;
  }

  switch (mGlue.stretchOrder)
  {
  case GlueOrder::Normal:
    stretch.normal += mGlue.stretch;
    break;
  case GlueOrder::Fil:
    stretch.fil += mGlue.stretch;
    break;
  case GlueOrder::Fill:
    stretch.fill += mGlue.stretch;
    break;
  case GlueOrder::Filll:
    stretch.filll += mGlue.stretch;
  }
}

} // namespace tex

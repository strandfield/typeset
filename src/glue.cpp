
#include "tex/glue.h"

namespace tex
{

Glue::Glue(float spc, float shrnk, float strtch, GlueOrder shrnkOrder, GlueOrder strtchOrder)
  : mGlue(GlueSpec{ spc, shrnk, strtch, shrnkOrder, strtchOrder })
{

}

} // namespace tex

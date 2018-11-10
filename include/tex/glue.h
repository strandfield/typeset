
#ifndef LIBLAYOUT_GLUE_H
#define LIBLAYOUT_GLUE_H

#include "tex/node.h"

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

  inline const GlueSpec & spec() const { return mGlue; }

private:
  const GlueSpec mGlue;
};

} // namespace tex

#endif // LIBLAYOUT_GLUE_H

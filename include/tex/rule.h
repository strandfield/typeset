
#ifndef LIBLAYOUT_RULE_H
#define LIBLAYOUT_RULE_H

#include "tex/box.h"

namespace tex
{

class LIBLAYOUT_API Rule final : public Box
{
public:
  Rule(float w, float h, float d);

  float height() const override;
  float depth() const override;
  float width() const override;

private:
  float mWidth;
  float mHeight;
  float mDepth;
};

} // namespace tex

#endif // LIBLAYOUT_RULE_H

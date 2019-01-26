// Copyright (C) 2018 Vincent Chambrin
// This file is part of the liblayout project
// For conditions of distribution and use, see copyright notice in LICENSE

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

LIBLAYOUT_API std::shared_ptr<Rule> hrule(float width, float height, float depth = 0.f);

} // namespace tex

#endif // LIBLAYOUT_RULE_H

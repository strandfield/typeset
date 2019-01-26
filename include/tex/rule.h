// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_RULE_H
#define LIBTYPESET_RULE_H

#include "tex/box.h"

namespace tex
{

class LIBTYPESET_API Rule final : public Box
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

LIBTYPESET_API std::shared_ptr<Rule> hrule(float width, float height, float depth = 0.f);

} // namespace tex

#endif // LIBTYPESET_RULE_H

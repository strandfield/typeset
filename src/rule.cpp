// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/rule.h"

namespace tex
{

Rule::Rule(float w, float h, float d)
  : Box(h, d, w)
{

}

std::shared_ptr<Rule> hrule(float width, float height, float depth)
{
  return std::make_shared<Rule>(width, height, depth);
}

} // namespace tex

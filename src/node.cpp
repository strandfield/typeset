// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/node.h"

#include "tex/box.h"
#include "tex/glue.h"
#include "tex/kern.h"
#include "tex/penalty.h"

namespace tex
{

bool Node::isBox() const
{
  return isDerivedFrom<Box>();
}

bool Node::isGlue() const
{
  return is<Glue>();
}

bool Node::isKern() const
{
  return is<Kern>();
}

bool Node::isPenalty() const
{
  return is<Penalty>();
}

} // namespace tex

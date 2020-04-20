// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_PARSHAPE_H
#define LIBTYPESET_PARSHAPE_H

#include "tex/defs.h"

#include <vector>

namespace tex
{

struct ParshapeSpec
{
  float indent;
  float length;
};

using Parshape = std::vector<ParshapeSpec>;

} // namespace tex

#endif // LIBTYPESET_PARSHAPE_H

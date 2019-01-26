// Copyright (C) 2018 Vincent Chambrin
// This file is part of the liblayout project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBLAYOUT_MATH_TYPESET_H
#define LIBLAYOUT_MATH_TYPESET_H

#include "tex/typeset.h"
#include "tex/math/mathlist.h"

namespace tex
{

LIBLAYOUT_API List mlist_to_hlist(MathList && mathlist, const Options & opts, int relpenalty = 500, int binoppenalty = 700);

} // namespace tex

#endif // LIBLAYOUT_MATH_TYPESET_H

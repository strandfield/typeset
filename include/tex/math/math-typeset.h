// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_MATH_TYPESET_H
#define LIBTYPESET_MATH_TYPESET_H

#include "tex/typeset.h"
#include "tex/math/mathlist.h"

namespace tex
{

LIBTYPESET_API List mlist_to_hlist(MathList && mathlist, const Options & opts, int relpenalty = 500, int binoppenalty = 700);

} // namespace tex

#endif // LIBTYPESET_MATH_TYPESET_H

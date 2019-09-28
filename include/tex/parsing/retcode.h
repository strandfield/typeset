// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_PARSING_RETCODE_H
#define LIBTYPESET_PARSING_RETCODE_H

#include "tex/defs.h"

namespace tex
{

namespace parsing
{

enum class RetCode
{
  Yield,
  Return,
  Await,
};

} // namespace parsing

} // namespace tex

#endif // LIBTYPESET_PARSING_RETCODE_H

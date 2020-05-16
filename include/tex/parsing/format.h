// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_PARSING_FORMAT_H
#define LIBTYPESET_PARSING_FORMAT_H

#include "tex/parsing/preprocessor.h"

namespace tex
{

namespace parsing
{

class LIBTYPESET_API Format
{
public:
  static std::vector<Macro> parse(const std::string& src);
};

} // namespace parsing

} // namespace tex

#endif // LIBTYPESET_PARSING_FORMAT_H

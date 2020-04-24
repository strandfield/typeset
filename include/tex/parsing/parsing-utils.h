// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_PARSINGUTILS_H
#define LIBTYPESET_PARSINGUTILS_H

#include <string>

namespace tex
{

namespace parsing
{

inline static bool is_digit(char c)
{
  return c >= '0' && c <= '9';
}

inline static bool is_lowercase_letter(char c)
{
  return c >= 'a' && c <= 'z';
}

} // namespace parsing

} // namespace tex

#endif // LIBTYPESET_PARSINGUTILS_H

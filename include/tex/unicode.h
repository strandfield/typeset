// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_UNICODE_H
#define LIBTYPESET_UNICODE_H

#include "tex/defs.h"

#include <string>

namespace tex
{

inline bool read_utf8_char(std::string::const_iterator begin, std::string::const_iterator end, std::string::const_iterator& output)
{
  if (!(*begin >> 7))
  {
    output = begin + 1;
    return true;
  }
  else if ((*begin >> 5) == 0x6)
  {
    ++begin;

    if ((*begin >> 6) == 0x2)
    {
      output = begin + 1;
      return true;
    }
  }
  else if ((*begin >> 4) == 0xE)
  {
    ++begin;

    if ((*begin >> 6) == 0x2 && (*(begin + 1) >> 6) == 0x2)
    {
      output = begin + 2;
      return true;
    }
  }
  else if ((*begin >> 3) == 0x1E)
  {
    ++begin;

    if ((*begin >> 6) == 0x2 && (*(begin + 1) >> 6) == 0x2 && (*(begin + 2) >> 6) == 0x2)
    {
      output = begin + 3;
      return true;
    }
  }

  return false;
}

inline bool is_utf8_char(const std::string& str)
{
  std::string::const_iterator it = str.end();
  return read_utf8_char(str.begin(), str.end(), it) && it == str.end();
}

inline bool is_utf8_string(const std::string& str)
{
  std::string::const_iterator it = str.begin();

  while (it != str.end())
  {
    if (!read_utf8_char(it, str.end(), it))
    {
      return false;
    }
  }

  return true;
}

} // namespace tex

#endif // LIBTYPESET_UNICODE_H

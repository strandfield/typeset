// Copyright (C) 2019-2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_UNICODE_H
#define LIBTYPESET_UNICODE_H

#include "tex/defs.h"

#include <array>
#include <cassert>
#include <string>

namespace tex
{

typedef int Character;

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

inline Character read_utf8_char(std::string::const_iterator& begin)
{
  if (!(*begin >> 7))
  {
    return *(begin++);
  }
  else if ((*begin >> 5) == 0x6)
  {
    Character ret = *(begin++) & 0x1F;
    assert((*begin >> 6) == 0x2);
    ret = (ret << 6) | (*(begin++) & 0x3F);
    return ret;
  }
  else if ((*begin >> 4) == 0xE)
  {
    Character ret = *(begin++) & 0xF;
    assert((*begin >> 6) == 0x2);
    ret = (ret << 6) | (*(begin++) & 0x3F);
    assert((*begin >> 6) == 0x2);
    ret = (ret << 6) | (*(begin++) & 0x3F);
    return ret;
  }
  else if ((*begin >> 3) == 0x1E)
  {
    Character ret = *(begin++) & 0x7;
    assert((*begin >> 6) == 0x2);
    ret = (ret << 6) | (*(begin++) & 0x3F);
    assert((*begin >> 6) == 0x2);
    ret = (ret << 6) | (*(begin++) & 0x3F);
    assert((*begin >> 6) == 0x2);
    ret = (ret << 6) | (*(begin++) & 0x3F);
    return ret;
  }
  else
  {
    assert(false);
    return 0;
  }
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

class LIBTYPESET_API Utf8Char
{
private:
  std::array<char, 5> m_chars;

public:
  explicit Utf8Char(Character c)
  {
    if (c <= 0x007F)
    {
      m_chars[0] = c;
      m_chars[1] = 0;
      m_chars[2] = 0;
      m_chars[3] = 0;
      m_chars[4] = 3;
    }
    else if (c <= 0x07FF)
    {
      m_chars[0] = (3 << 6) | ((c >> 6) & 0x3F);
      m_chars[1] = (1 << 7) | (c & 0x3F);
      m_chars[2] = 0;
      m_chars[3] = 0;
      m_chars[4] = 2;
    }
    else if (c <= 0xFFFF)
    {
      m_chars[0] = (7 << 5) | ((c >> 12) & 0x3F);
      m_chars[1] = (1 << 7) | ((c >> 6) & 0x3F);
      m_chars[2] = (1 << 7) | (c & 0x3F);
      m_chars[3] = 0;
      m_chars[4] = 1;
    }
    else
    {
      m_chars[0] = (15 << 4) | ((c >> 18) & 0x3F);
      m_chars[1] = (1 << 7) | ((c >> 12) & 0x3F);
      m_chars[2] = (1 << 7) | ((c >> 6) & 0x3F);
      m_chars[3] = (1 << 7) | (c & 0x3F);
      m_chars[4] = 0;
    }
  }

  const char* data() const { return m_chars.data(); }
  size_t size() const { return 4 - m_chars[4]; }
};

} // namespace tex

#endif // LIBTYPESET_UNICODE_H

// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_APPCOMMON_CHARBUFFER_H
#define LIBTYPESET_APPCOMMON_CHARBUFFER_H

#include "tex/unicode.h"

class CharBuffer
{
public:

  void write(char c)
  {
    m_buffer.push_back(c);
  }

  bool ready() const
  {
    return tex::is_utf8_char(m_buffer);
  }

  tex::Character read()
  {
    tex::Character c = tex::read_utf8_char(m_buffer.cbegin());
    m_buffer.clear();
    return c;
  }


private:
  std::string m_buffer;
};

#endif // LIBTYPESET_APPCOMMON_CHARBUFFER_H

// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "inputstream.h"

void InputStream::removeCRLF(std::string& str)
{
  size_t w = 0;

  for (size_t r(0); r < str.size(); ++r)
  {
    if (str[r] != '\r')
    {
      str[w++] = str[r];
    }
  }

  str.resize(w);
}

std::pair<size_t, size_t> InputStream::position() const
{
  size_t line = 0;
  size_t col = 0;

  size_t i = 0;

  while (i < pos)
  {
    if (text.at(i++) == '\n')
    {
      ++line;
      col = 0;
    }
    else
    {
      ++col;
    }
  }

  return std::make_pair(line, col);
}

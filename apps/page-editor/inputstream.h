// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef TYPESET_PAGEEDITOR_INPUTSTREAM_H
#define TYPESET_PAGEEDITOR_INPUTSTREAM_H

#include "tex/token.h"

#include <string>
#include <utility>

class InputStream
{
public:
  InputStream();

  InputStream(std::string content);

  bool atEnd() const;

  char read();
  char peek() const;

  static void removeCRLF(std::string& str);

  std::pair<size_t, size_t> position() const;

public:
  std::string text;
  size_t pos;
};

inline InputStream::InputStream()
  : pos(0)
{

}

inline InputStream::InputStream(std::string content)
  : text(std::move(content)),
    pos(0)
{

}

inline bool InputStream::atEnd() const
{
  return pos == text.size();
}

inline char InputStream::read()
{
  return text[pos++];
}

inline char InputStream::peek() const
{
  return text.at(pos);
}


#endif // TYPESET_PAGEEDITOR_INPUTSTREAM_H

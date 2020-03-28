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
private:
  const char* m_src;
  const char* m_bytecode;

public:
  Format(const char* src, const char* bytecode);

  const char* source() const;
  const char* bytecode() const;

  static std::vector<Macro> load(const Format& fmt);
};

} // namespace parsing

} // namespace tex

#endif // LIBTYPESET_PARSING_FORMAT_H

// Copyright (C) 2019-2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_SYMBOL_H
#define LIBTYPESET_SYMBOL_H

#include "tex/node.h"
#include "tex/unicode.h"

#include <string>

namespace tex
{

class LIBTYPESET_API Symbol : public Node
{

};

class LIBTYPESET_API TextSymbol : public Symbol
{
  std::string m_text;
public:
  explicit TextSymbol(const std::string& str) : m_text{str} { }
  explicit TextSymbol(std::string&& str) : m_text{std::move(str)} {}
  ~TextSymbol() = default;

  const std::string& text() const { return m_text; }
};

class LIBTYPESET_API MathSymbol : public Symbol
{
private:
  Character m_char;

public:
  explicit MathSymbol(Character c) : m_char(c) { }

  Character character() const { return m_char;  }
};

} // namespace tex

#endif // LIBTYPESET_SYMBOL_H

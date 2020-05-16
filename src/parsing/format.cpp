// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/parsing/format.h"

#include "tex/lexer.h"
#include "tex/parsing/preprocessor.h"

#include <cassert>

namespace tex
{

namespace parsing
{

std::vector<Macro> Format::parse(const std::string& src)
{
  parsing::Lexer lex;
  lex.catcodes()['@'] = parsing::CharCategory::Letter; // \makeatletter

  for (char c : src)
  {
    lex.write(c);
  }

  parsing::Preprocessor preproc;

  for (Token& tok : lex.output())
  {
    preproc.write(std::move(tok));
  }

  std::vector<Macro> result;

  for (const auto& blocks : preproc.macros())
  {
    for (const auto& e : blocks.macros)
    {
      result.push_back(e.second);
    }
  }

  return result;
}

} // namespace parsing

} // namespace tex

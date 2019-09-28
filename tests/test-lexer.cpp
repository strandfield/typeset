// Copyright (C) 2019 Vincent Chambrin
// This file is part of the typeset project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "test.h"

#include "tex/lexer.h"

void test_lexer()
{
  using namespace tex;

  parsing::Lexer lex;
  std::vector<parsing::Token>& toks = lex.output();

  auto write = [&lex](const std::string& str) ->void {
    for (char c : str)
      lex.write(c);
  };

  write("a\\cmd b\n\n#1##");

  ASSERT(toks.size() == 7);
  ASSERT(toks.front().isCharacterToken());
  ASSERT(toks.at(1).isControlSequence());
  ASSERT(toks.at(1).controlSequence() == "cmd");
  ASSERT(toks.at(2).isCharacterToken());
  ASSERT(toks.at(3).isCharacterToken());
  ASSERT(toks.at(3).characterToken().value == ' ');
  ASSERT(toks.at(4).isControlSequence());
  ASSERT(toks.at(4).controlSequence() == "par");
  ASSERT(toks.at(5).isParameterToken());
  ASSERT(toks.at(5).parameterNumber() == 1);
  ASSERT(toks.at(6).isCharacterToken());
  ASSERT(toks.at(6).characterToken().value == '#');

  toks.clear();

  write("Hello   World!");
  ASSERT(toks.size() == 12);
  ASSERT(toks.at(4).characterToken().value == 'o');
  ASSERT(toks.at(5).characterToken().value == ' ');
  ASSERT(toks.at(6).characterToken().value == 'W');

  toks.clear();

  write("K % This is a comment \n");
  ASSERT(toks.size() == 2);
  ASSERT(toks.at(0).characterToken().value == 'K');
  ASSERT(toks.at(1).characterToken().value == ' ');

  toks.clear();

  write("L");
  ASSERT(toks.size() == 1);
  ASSERT(toks.at(0).characterToken().value == 'L');
}

// Copyright (C) 2019 Vincent Chambrin
// This file is part of the typeset project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "catch.hpp"

#include "tex/lexer.h"

TEST_CASE("Tokens can be produced by the Lexer", "[lexer]")
{
  using namespace tex;

  parsing::Lexer lex;
  std::vector<parsing::Token>& toks = lex.output();

  auto write = [&lex](const std::string& str) ->void {
    for (char c : str)
      lex.write(c);
  };

  write("a\\cmd b\n\n#1##");

  REQUIRE(toks.size() == 7);
  REQUIRE(toks.front().isCharacterToken());
  REQUIRE(toks.at(1).isControlSequence());
  REQUIRE(toks.at(1).controlSequence() == "cmd");
  REQUIRE(toks.at(2).isCharacterToken());
  REQUIRE(toks.at(3).isCharacterToken());
  REQUIRE(toks.at(3).characterToken().value == ' ');
  REQUIRE(toks.at(4).isControlSequence());
  REQUIRE(toks.at(4).controlSequence() == "par");
  REQUIRE(toks.at(5).isParameterToken());
  REQUIRE(toks.at(5).parameterNumber() == 1);
  REQUIRE(toks.at(6).isCharacterToken());
  REQUIRE(toks.at(6).characterToken().value == '#');

  toks.clear();

  write("Hello   World!");
  REQUIRE(toks.size() == 12);
  REQUIRE(toks.at(4).characterToken().value == 'o');
  REQUIRE(toks.at(5).characterToken().value == ' ');
  REQUIRE(toks.at(6).characterToken().value == 'W');

  toks.clear();

  write("K % This is a comment \n");
  REQUIRE(toks.size() == 2);
  REQUIRE(toks.at(0).characterToken().value == 'K');
  REQUIRE(toks.at(1).characterToken().value == ' ');

  toks.clear();

  write("L");
  REQUIRE(toks.size() == 1);
  REQUIRE(toks.at(0).characterToken().value == 'L');
}

// Copyright (C) 2019 Vincent Chambrin
// This file is part of the typeset project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "test.h"

#include "tex/parser.h"

void test_parser()
{
  using namespace tex;

  parsing::Lexer lex;
  std::vector<parsing::Token>& toks = lex.output();

  auto write = [&lex](const std::string& str) ->void {
    for (char c : str)
      lex.write(c);
  };

  write("Hello {Bob} !\n\n");

  parsing::ParserMachine parser;

  parser.write(toks);

  std::vector<std::shared_ptr<typesetting::TypesettingInstruction>>& instr = parser.output();

  ASSERT(instr.size() == 15);
  ASSERT(typeid(*instr.at(5)) == typeid(typesetting::InsertInterwordGlue));
  ASSERT(typeid(*instr.at(6)) == typeid(typesetting::EnterGroup));
  ASSERT(typeid(*instr.at(7)) == typeid(typesetting::TypesetText));
  ASSERT(typeid(*instr.at(8)) == typeid(typesetting::TypesetText));
  ASSERT(typeid(*instr.at(9)) == typeid(typesetting::TypesetText));
  ASSERT(typeid(*instr.at(10)) == typeid(typesetting::LeaveGroup));
  ASSERT(typeid(*instr.at(11)) == typeid(typesetting::InsertInterwordGlue));
  ASSERT(typeid(*instr.at(12)) == typeid(typesetting::TypesetText));
  ASSERT(typeid(*instr.at(13)) == typeid(typesetting::InsertInterwordGlue));
  ASSERT(typeid(*instr.at(14)) == typeid(typesetting::EndParagraph));
}

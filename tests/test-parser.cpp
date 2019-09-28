// Copyright (C) 2019 Vincent Chambrin
// This file is part of the typeset project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "test.h"

#include "test-typeset.h"

#include "tex/parser.h"

#include "tex/hbox.h"

#include "tex/parsing/verticalmode.h"
#include "tex/parsing/typesetting-machine.h"

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

void test_machine_1()
{
  using namespace tex;

  auto engine = std::make_shared<TestTypesetEngine>();

  parsing::TypesettingMachine machine{ engine, tex::Options{engine} };

  machine.inputStream().push_back(parsing::InputStream::Document::fromString("Hello {Bob} !\n\n"));

  while (!machine.inputStream().atEnd())
  {
    machine.advance();
  }

  auto* vm = static_cast<tex::parsing::VerticalMode*>(machine.modes().front().get());

  tex::List vlist = vm->vlist();

  ASSERT(vlist.size() == 1);
  ASSERT(vlist.front()->isDerivedFrom<tex::HBox>());
}

void test_machine_2()
{
  using namespace tex;

  auto engine = std::make_shared<TestTypesetEngine>();

  parsing::TypesettingMachine machine{ engine, tex::Options{engine} };

  machine.inputStream().push_back(parsing::InputStream::Document::fromString("\\def\\foo{Hello World!} \\foo \n\n"));

  while (!machine.inputStream().atEnd())
  {
    machine.advance();
  }

  auto* vm = static_cast<tex::parsing::VerticalMode*>(machine.modes().front().get());

  tex::List vlist = vm->vlist();

  ASSERT(vlist.size() == 1);
  ASSERT(vlist.front()->isDerivedFrom<tex::HBox>());
}


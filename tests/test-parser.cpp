// Copyright (C) 2019 Vincent Chambrin
// This file is part of the typeset project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "test.h"

#include "test-typeset.h"

#include "tex/hbox.h"

#include "tex/parsing/verticalmode.h"
#include "tex/parsing/typesetting-machine.h"

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

  {
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

  {
    parsing::TypesettingMachine machine{ engine, tex::Options{engine} };

    machine.inputStream().push_back(parsing::InputStream::Document::fromString("\\def\\foo#1{Hello #1!} \\foo{Bob} \n\n"));

    while (!machine.inputStream().atEnd())
    {
      machine.advance();
    }

    auto* vm = static_cast<tex::parsing::VerticalMode*>(machine.modes().front().get());

    tex::List vlist = vm->vlist();

    ASSERT(vlist.size() == 1);
    ASSERT(vlist.front()->isDerivedFrom<tex::HBox>());
  }
}


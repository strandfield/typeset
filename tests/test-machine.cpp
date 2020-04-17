// Copyright (C) 2019-2020 Vincent Chambrin
// This file is part of the typeset project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "catch.hpp"

#include "test-typeset.h"

#include "tex/hbox.h"

#include "tex/parsing/verticalmode.h"
#include "tex/parsing/typesetting-machine.h"

TEST_CASE("Grouping doesn't alter the ouput", "[parsing-machine]")
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

  REQUIRE(vlist.size() == 1);
  REQUIRE(vlist.front()->isHBox());
}

TEST_CASE("Macros are correctly expanded", "[parsing-machine]")
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

    REQUIRE(vlist.size() == 1);
    REQUIRE(vlist.front()->isHBox());
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

    REQUIRE(vlist.size() == 1);
    REQUIRE(vlist.front()->isHBox());
  }
}

TEST_CASE("Inline math is handled correctly", "[parsing-machine]")
{
  using namespace tex;

  auto engine = std::make_shared<TestTypesetEngine>();

  parsing::TypesettingMachine machine{ engine, tex::Options{engine} };

  machine.inputStream().push_back(parsing::InputStream::Document::fromString("Let $x_n > {1 \\over n}$ and $y = 0$...\n\n"));

  while (!machine.inputStream().atEnd())
  {
    machine.advance();
  }

  auto* vm = static_cast<tex::parsing::VerticalMode*>(machine.modes().front().get());

  tex::List vlist = vm->vlist();

  REQUIRE(vlist.size() == 1);
  REQUIRE(vlist.front()->isHBox());
  /// TODO: add some real check
}
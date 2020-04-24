// Copyright (C) 2020 Vincent Chambrin
// This file is part of the typeset project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "catch.hpp"

#include "tex/parsing/glueparser.h"
#include "tex/parsing/kernparser.h"

template<typename T>
static void write_chars(T& parser, const std::string& str)
{
  for (size_t i(0); i < str.size(); ++i)
  {
    char c = str.at(i);
    parser.write(c);
  }
}

TEST_CASE("The parser can process a simple finite dimen", "[glue-parsing]")
{
  using namespace tex;

  parsing::DimenParser parser;
  write_chars(parser, "+20pt");

  Dimen d = parser.finish();

  REQUIRE(d.unit() == Unit::Pt);
  REQUIRE(d.value() == 20.f);
}

TEST_CASE("The parser can process a finite dimen with a minus sign", "[glue-parsing]")
{
  using namespace tex;

  parsing::DimenParser parser;
  write_chars(parser, "-20em");

  Dimen d = parser.finish();

  REQUIRE(d.unit() == Unit::Em);
  REQUIRE(d.value() == -20.f);
}

TEST_CASE("The parser can process an infinite decimal dimen", "[glue-parsing]")
{
  using namespace tex;

  parsing::DimenParser parser;
  write_chars(parser, "-0.5fill");

  Dimen d = parser.finish();

  REQUIRE(!d.isFinite());
  REQUIRE(d.unit() == Unit::Fill);
  REQUIRE(d.value() == -0.5f);
}

TEST_CASE("The parser can process a simple glue", "[glue-parsing]")
{
  using namespace tex;

  UnitSystem us;
  us.em = 2.f;
  us.ex = 0.5f;
  us.pt = 1.f;

  parsing::GlueParser parser{ us };
  write_chars(parser, "1em");

  std::shared_ptr<Glue> g = parser.finish();

  REQUIRE(g->space() == 2.f);
  REQUIRE(g->stretch() == 0.f);
  REQUIRE(g->shrink() == 0.f);
}

TEST_CASE("The parser can process a glue with finite stretch & shrink", "[glue-parsing]")
{
  using namespace tex;

  UnitSystem us;
  us.em = 2.f;
  us.ex = 0.5f;
  us.pt = 1.f;

  parsing::GlueParser parser{ us };
  write_chars(parser, "1ex plus 2pt minus 3em");

  std::shared_ptr<Glue> g = parser.finish();

  REQUIRE(g->space() == 0.5f);
  REQUIRE(g->stretch() == 2.f);
  REQUIRE(g->shrink() == 6.f);
}

TEST_CASE("The parser can process a glue with infinite stretch & shrink", "[glue-parsing]")
{
  using namespace tex;

  UnitSystem us;
  us.em = 2.f;
  us.ex = 0.5f;
  us.pt = 1.f;

  parsing::GlueParser parser{ us };
  write_chars(parser, "1pc plus 1fil minus 2fill");

  std::shared_ptr<Glue> g = parser.finish();

  REQUIRE(g->space() == 12.f);
  REQUIRE(g->stretch() == 1.f);
  REQUIRE(g->shrink() == 2.f);
  REQUIRE(g->stretchOrder() == GlueOrder::Fil);
  REQUIRE(g->shrinkOrder() == GlueOrder::Fill);
}

TEST_CASE("The parser can process a glue with a trailing space", "[glue-parsing]")
{
  using namespace tex;

  UnitSystem us;
  us.em = 2.f;
  us.ex = 0.5f;
  us.pt = 1.f;

  parsing::GlueParser parser{ us };
  write_chars(parser, "1pc ");

  std::shared_ptr<Glue> g = parser.finish();

  REQUIRE(g->space() == 12.f);
}

TEST_CASE("The parser can process a kern", "[glue-parsing]")
{
  using namespace tex;

  UnitSystem us;
  us.em = 2.f;
  us.ex = 0.5f;
  us.pt = 1.f;

  parsing::KernParser parser{ us };
  write_chars(parser, "1pc ");

  std::shared_ptr<Kern> k = parser.finish();

  REQUIRE(k->space() == 12.f);
}

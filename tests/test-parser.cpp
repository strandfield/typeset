// Copyright (C) 2019-2020 Vincent Chambrin
// This file is part of the typeset project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "catch.hpp"

#include "tex/math/fraction.h"
#include "tex/math/matrix.h"
#include "tex/math/root.h"

#include "tex/parsing/mathparserfrontend.h"

TEST_CASE("The parser can process a simple atom", "[math-parsing]")
{
  using namespace tex;

  parsing::MathParserFrontend parser;
  parser.writeChar('a');
  parser.finish();

  REQUIRE(parser.output().size() == 1);
  REQUIRE(parser.output().front()->is<math::Atom>());
}

TEST_CASE("The parser handles \\over", "[math-parsing]")
{
  using namespace tex;
  using namespace parsing;

  parsing::MathParserFrontend parser;
  parser.writeChar('1');
  parser.writeChar('+');
  parser.writeChar('2');
  parser.writeControlSequence("over");
  parser.writeChar('5');
  parser.finish();

  REQUIRE(parser.output().size() == 1);
  REQUIRE(parser.output().front()->is<math::Fraction>());
}

TEST_CASE("The parser handles \\left and \\right", "[math-parsing]")
{
  using namespace tex;
  using namespace parsing;

  parsing::MathParserFrontend parser;
  parser.writeControlSequence("left");
  parser.writeChar('(');
  parser.writeChar('a');
  parser.writeChar('+');
  parser.writeChar('b');
  parser.writeControlSequence("right");
  parser.writeChar(')');
  parser.finish();

  REQUIRE(parser.output().size() == 1);
  REQUIRE(parser.output().front()->is<math::Atom>());
  REQUIRE(parser.output().front()->as<math::Atom>().type() == math::Atom::Inner);
}

TEST_CASE("The parser handles a superscript", "[math-parsing]")
{
  using namespace tex;
  using namespace parsing;

  parsing::MathParserFrontend parser;
  parser.writeChar('x');
  parser.beginSuperscript();
  parser.writeChar('2');
  parser.writeChar('+');
  parser.writeChar('2');
  parser.writeChar('x');
  parser.writeChar('+');
  parser.writeChar('1');
  parser.finish();

  REQUIRE(parser.output().size() == 6);
  REQUIRE(parser.output().front()->is<math::Atom>());
  REQUIRE(parser.output().front()->as<math::Atom>().type() == math::Atom::Ord);
  REQUIRE(parser.output().front()->as<math::Atom>().superscript() != nullptr);
}

TEST_CASE("The parser handles a square root", "[math-parsing]")
{
  using namespace tex;
  using namespace parsing;

  {
    // \sqrt{1 + \sqrt 2}

    parsing::MathParserFrontend parser;
    parser.writeControlSequence("sqrt");
    parser.beginMathList();
    parser.writeChar('1');
    parser.writeChar('+');
    parser.writeControlSequence("sqrt");
    parser.writeChar('2');
    parser.endMathList();
    parser.finish();

    REQUIRE(parser.output().size() == 1);
    REQUIRE(parser.output().front()->is<math::Atom>());
    REQUIRE(parser.output().front()->as<math::Atom>().type() == math::Atom::Rad);
  }

  {
    // \sqrt[3]{n}

    parsing::MathParserFrontend parser;
    parser.writeControlSequence("sqrt");
    parser.writeChar('[');
    parser.writeChar('3');
    parser.writeChar(']');
    parser.beginMathList();
    parser.writeChar('n');
    parser.endMathList();
    parser.finish();

    REQUIRE(parser.output().size() == 1);
    REQUIRE(parser.output().front()->is<math::Root>());
    REQUIRE(parser.output().front()->as<math::Root>().degree().size() == 1);
  }
}

TEST_CASE("The parser handles a \\frac", "[math-parsing]")
{
  using namespace tex;
  using namespace parsing;

  {
    // frac{a}{b}

    parsing::MathParserFrontend parser;
    parser.writeControlSequence("frac");
    parser.beginMathList();
    parser.writeChar('a');
    parser.endMathList();
    parser.beginMathList();
    parser.writeChar('b');
    parser.endMathList();
    parser.finish();

    REQUIRE(parser.output().size() == 1);
    REQUIRE(parser.output().front()->is<math::Fraction>());
  }

}

TEST_CASE("The parser handles a \\matrix", "[math-parsing]")
{
  using namespace tex;
  using namespace parsing;

  {
    // \matrix{ a & b \cr c & d }

    parsing::MathParserFrontend parser;
    parser.writeControlSequence("matrix");
    parser.beginMathList();
    parser.writeChar('a');
    parser.alignmentTab();
    parser.writeChar('b');
    parser.writeControlSequence("cr");
    parser.writeChar('c');
    parser.alignmentTab();
    parser.writeChar('d');
    parser.endMathList();
    parser.finish();

    REQUIRE(parser.output().size() == 1);
    REQUIRE(parser.output().front()->is<math::Matrix>());
  }
}

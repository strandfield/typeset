// Copyright (C) 2019 Vincent Chambrin
// This file is part of the typeset project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "test.h"

#include "tex/math/fraction.h"
#include "tex/math/matrix.h"
#include "tex/math/root.h"

#include "tex/parsing/mathparserfrontend.h"

TEST_CASE(test_parser_1)
{
  using namespace tex;

  parsing::MathParserFrontend parser;
  parser.writeChar('a');
  parser.finish();

  ASSERT(parser.output().size() == 1);
  ASSERT(parser.output().front()->is<math::Atom>());
}

TEST_CASE(test_parser_2)
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

  ASSERT(parser.output().size() == 1);
  ASSERT(parser.output().front()->is<math::Fraction>());
}

TEST_CASE(test_parser_3)
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

  ASSERT(parser.output().size() == 1);
  ASSERT(parser.output().front()->is<math::Atom>());
  ASSERT(parser.output().front()->as<math::Atom>().type() == math::Atom::Inner);
}

TEST_CASE(test_parser_4)
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

  ASSERT(parser.output().size() == 6);
  ASSERT(parser.output().front()->is<math::Atom>());
  ASSERT(parser.output().front()->as<math::Atom>().type() == math::Atom::Ord);
  ASSERT(parser.output().front()->as<math::Atom>().superscript() != nullptr);
}

TEST_CASE(test_parser_sqrt)
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

    ASSERT(parser.output().size() == 1);
    ASSERT(parser.output().front()->is<math::Atom>());
    ASSERT(parser.output().front()->as<math::Atom>().type() == math::Atom::Rad);
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

    ASSERT(parser.output().size() == 1);
    ASSERT(parser.output().front()->is<math::Root>());
    ASSERT(parser.output().front()->as<math::Root>().degree().size() == 1);
  }
}

TEST_CASE(test_parser_frac)
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

    ASSERT(parser.output().size() == 1);
    ASSERT(parser.output().front()->is<math::Fraction>());
  }

}

TEST_CASE(test_parser_matrix)
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

    ASSERT(parser.output().size() == 1);
    ASSERT(parser.output().front()->is<math::Matrix>());
  }
}

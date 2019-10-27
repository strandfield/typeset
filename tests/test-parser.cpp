// Copyright (C) 2019 Vincent Chambrin
// This file is part of the typeset project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "test.h"

#include "tex/math/fraction.h"

#include "tex/parsing/mathparser.h"

void test_parser_1()
{
  using namespace tex;

  parsing::MathParser parser;
  parser.writeSymbol("a");
  parser.finish();

  ASSERT(parser.output().size() == 1);
  ASSERT(parser.output().front()->is<math::Atom>());
}

void test_parser_2()
{
  using namespace tex;
  using namespace parsing;

  parsing::MathParser parser;
  parser.writeSymbol("1");
  parser.writeSymbol("+");
  parser.writeSymbol("2");
  parser.writeControlSequence(MathParser::CS::OVER);
  parser.writeSymbol("5");
  parser.finish();

  ASSERT(parser.output().size() == 1);
  ASSERT(parser.output().front()->is<math::Fraction>());
}

void test_parser_3()
{
  using namespace tex;
  using namespace parsing;

  parsing::MathParser parser;
  parser.writeControlSequence(MathParser::CS::LEFT);
  parser.writeSymbol("(");
  parser.writeSymbol("a");
  parser.writeSymbol("+");
  parser.writeSymbol("b");
  parser.writeControlSequence(MathParser::CS::RIGHT);
  parser.writeSymbol(")");
  parser.finish();

  ASSERT(parser.output().size() == 1);
  ASSERT(parser.output().front()->is<math::Atom>());
  ASSERT(parser.output().front()->as<math::Atom>().type() == math::Atom::Inner);
}

void test_parser_4()
{
  using namespace tex;
  using namespace parsing;

  parsing::MathParser parser;
  parser.writeSymbol("x");
  parser.beginSuperscript();
  parser.writeSymbol("2");
  parser.writeSymbol("+");
  parser.writeSymbol("2");
  parser.writeSymbol("x");
  parser.writeSymbol("+");
  parser.writeSymbol("1");
  parser.finish();

  ASSERT(parser.output().size() == 6);
  ASSERT(parser.output().front()->is<math::Atom>());
  ASSERT(parser.output().front()->as<math::Atom>().type() == math::Atom::Ord);
  ASSERT(parser.output().front()->as<math::Atom>().superscript() != nullptr);
}

void test_parser_sqrt()
{
  using namespace tex;
  using namespace parsing;

  parsing::MathParser parser;
  parser.writeControlSequence(MathParser::CS::SQRT);
  parser.beginMathList();
  parser.writeSymbol("1");
  parser.writeSymbol("+");
  parser.writeControlSequence(MathParser::CS::SQRT);
  parser.writeSymbol("2");
  parser.endMathList();
  parser.finish();

  ASSERT(parser.output().size() == 1);
  ASSERT(parser.output().front()->is<math::Atom>());
  ASSERT(parser.output().front()->as<math::Atom>().type() == math::Atom::Rad);
}
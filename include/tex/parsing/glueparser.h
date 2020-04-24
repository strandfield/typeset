// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_GLUEPARSER_H
#define LIBTYPESET_GLUEPARSER_H

#include "tex/glue.h"
#include "tex/units.h"

#include "tex/parsing/dimenparser.h"

#include <string>

namespace tex
{

namespace parsing
{

class LIBTYPESET_API GlueParser
{
public:

  enum class State
  {
    ParsingSpace,
    ParsedSpace,
    ParsingPlus,
    ParsedPlus,
    ParsingPlusDimen,
    ParsedPlusDimen,
    ParsingMinus,
    ParsedMinus,
    ParsingMinusDimen,
    Finished,
  };

private:
  UnitSystem m_unitsystem;
  State m_state = State::ParsingSpace;
  DimenParser m_dimen_parser;
  GlueSpec m_gluespec;
  int m_wordparsing = 0;

public:

  explicit GlueParser(UnitSystem us);

  const State& state() const { return m_state; }

  static void writeGlueOrder(GlueOrder& go, Unit infunit);

  void write(char c);

  std::shared_ptr<Glue> finish();

protected:

  void write(float& gval, GlueOrder& go, const Dimen& d);
};

} // namespace parsing

} // namespace tex

#endif // LIBTYPESET_GLUEPARSER_H

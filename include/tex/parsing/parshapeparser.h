// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_PARSHAPEPARSER_H
#define LIBTYPESET_PARSHAPEPARSER_H

#include "tex/parshape.h"
#include "tex/units.h"

#include "tex/parsing/dimenparser.h"

#include <string>

namespace tex
{

namespace parsing
{

class LIBTYPESET_API ParshapeParser
{
public:

  enum class State
  {
    ParsingOptionalAssign,
    ParsingNum,
    ParsingIndent,
    ParsingLength,
    Finished,
  };

private:
  UnitSystem m_unitsystem;
  State m_state = State::ParsingOptionalAssign;
  int m_num = 0;
  std::vector<ParshapeSpec> m_specs;
  DimenParser m_dimen_parser;

public:

  explicit ParshapeParser(UnitSystem us);

  const State& state() const { return m_state; }

  void write(char c);

  bool isFinished() const;

  Parshape finish();

protected:
};

} // namespace parsing

} // namespace tex

#endif // LIBTYPESET_PARSHAPEPARSER_H

// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_KERNPARSER_H
#define LIBTYPESET_KERNPARSER_H

#include "tex/kern.h"
#include "tex/units.h"

#include "tex/parsing/dimenparser.h"

#include <string>

namespace tex
{

namespace parsing
{

class LIBTYPESET_API KernParser
{
public:

  enum class State
  {
    ParsingSpace,
    Finished,
  };

private:
  UnitSystem m_unitsystem;
  State m_state = State::ParsingSpace;
  DimenParser m_dimen_parser;

public:

  explicit KernParser(UnitSystem us);

  const State& state() const { return m_state; }

  void write(char c);

  std::shared_ptr<Kern> finish();

protected:
};

} // namespace parsing

} // namespace tex

#endif // LIBTYPESET_KERNPARSER_H

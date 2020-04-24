// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_DIMENPARSER_H
#define LIBTYPESET_DIMENPARSER_H

#include "tex/dimen.h"

#include <string>

namespace tex
{

namespace parsing
{

class LIBTYPESET_API DimenParser
{
public:

  enum class State
  {
    ParseOptionalSign,
    ParsingFactor,
    ParsingUnit,
    Finished,
  };

private:
  State m_state = State::ParseOptionalSign;
  char m_sign = '+';
  std::string m_factor;
  bool m_factor_is_decimal = false;
  std::string m_unit;

public:
  DimenParser();

  const State& state() const { return m_state; }

  void reset();

  void write(char c);

  bool hasResult() const;
  Dimen finish();

protected:
};

} // namespace parsing

} // namespace tex

#endif // LIBTYPESET_DIMENPARSER_H

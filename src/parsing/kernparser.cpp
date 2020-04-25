// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/parsing/kernparser.h"

namespace tex
{

namespace parsing
{

KernParser::KernParser(UnitSystem us)
  : m_unitsystem(us)
{

}

void KernParser::write(char c)
{
  switch (state())
  {
  case State::ParsingSpace:
  {
    m_dimen_parser.write(c);

    if (m_dimen_parser.state() == DimenParser::State::Finished)
    {
      m_state = State::Finished;
    }

    return;
  }
  break;
  case State::Finished:
  {
    if (c != ' ')
      throw std::runtime_error{ "KernParser::write()" };

    return;
  }
  break;
  }
}

bool KernParser::isFinished()
{
  return m_state == State::Finished;
}

std::shared_ptr<Kern> KernParser::finish()
{
  Dimen d = m_dimen_parser.finish();

  if (!d.isFinite())
    throw std::runtime_error{ "KernParser::finish()" };

  return tex::kern(d(m_unitsystem));
}

} // namespace parsing

} // namespace tex

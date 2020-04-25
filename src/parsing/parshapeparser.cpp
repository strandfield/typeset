// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/parsing/parshapeparser.h"

#include "tex/parsing/parsing-utils.h"

namespace tex
{

namespace parsing
{

ParshapeParser::ParshapeParser(UnitSystem us)
  : m_unitsystem(us)
{

}

void ParshapeParser::write(char c)
{
  switch (state())
  {
  case State::ParsingOptionalAssign:
  {
    if (c == '=')
    {
      m_state = State::ParsingNum;
    }
    else if (is_digit(c))
    {
      m_state = State::ParsingNum;
      m_num = c - '0';
    }
    else if (!is_digit(c))
    {
      throw std::runtime_error{ "ParshapeParser::write()" };
    }

    return;
  }
  break;
  case State::ParsingNum:
  {
    if (is_digit(c))
    {
      m_num = m_num * 10 + (c - '0');
    }
    else if (c == ' ')
    {
      if (m_num == 0)
      {
        m_state = State::Finished;
        return;
      }

      m_state = State::ParsingIndent;
    }
    else
    {
      throw std::runtime_error{ "ParshapeParser::write()" };
    }

    return;
  }
  break;
  case State::ParsingIndent:
  {
    m_dimen_parser.write(c);

    if (m_dimen_parser.isFinished())
    {
      Dimen d = m_dimen_parser.finish();
      m_specs.emplace_back();
      m_specs.back().indent = d(m_unitsystem);
      m_state = State::ParsingLength;
      m_dimen_parser.reset();
    }

    return;
  }
  break;
  case State::ParsingLength:
  {
    m_dimen_parser.write(c);

    if (m_dimen_parser.isFinished())
    {
      Dimen d = m_dimen_parser.finish();
      m_specs.back().length = d(m_unitsystem);
      
      if (m_specs.size() == static_cast<size_t>(m_num))
      {
        m_state = State::Finished;
      }
      else
      {
        m_dimen_parser.reset();
        m_state = State::ParsingIndent;
      }
    }

    return;
  }
  break;
  case State::Finished:
  {
    if (c != ' ')
      throw std::runtime_error{ "ParshapeParser::write()" };

    return;
  }
  break;
  }
}

bool ParshapeParser::isFinished() const
{
  return m_state == State::Finished;
}

Parshape ParshapeParser::finish()
{
  if (isFinished())
    return m_specs;

  if (m_state == State::ParsingNum && m_num == 0)
  {
    return {};
  }
  else if (m_state == State::ParsingLength && static_cast<size_t>(m_num) == m_specs.size())
  {
    Dimen d = m_dimen_parser.finish();
    m_specs.back().length = d(m_unitsystem);
    return m_specs;
  }
  else
  {
    throw std::runtime_error{ "ParshapeParser::finish()" };
  }
}

} // namespace parsing

} // namespace tex

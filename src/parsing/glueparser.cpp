// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/parsing/glueparser.h"

namespace tex
{

namespace parsing
{

GlueParser::GlueParser(UnitSystem us)
  : m_unitsystem(us),
    m_gluespec{ 0.f, 0.f, 0.f, GlueOrder::Normal, GlueOrder::Normal }
{

}

void GlueParser::writeGlueOrder(GlueOrder& go, Unit infunit)
{
  switch (infunit)
  {
  case Unit::Fil:
    go = GlueOrder::Fil;
    break;
  case Unit::Fill:
    go = GlueOrder::Fill;
    break;
  case Unit::Filll:
    go = GlueOrder::Filll;
    break;
  default:
    assert(false);
    break;
  }
}

void GlueParser::write(float& gval, GlueOrder& go, const Dimen& d)
{
  if (!d.isFinite())
  {
    gval = d.value();
    writeGlueOrder(go, d.unit());
  }
  else
  {
    gval = d(m_unitsystem);
    go = GlueOrder::Normal;
  }
}

void GlueParser::write(char c)
{
  switch (state())
  {
  case State::ParsingSpace:
  {
    m_dimen_parser.write(c);

    if (m_dimen_parser.state() == DimenParser::State::Finished)
    {
      Dimen d = m_dimen_parser.finish();
      
      if (!d.isFinite())
        throw std::runtime_error{ "GlueParser::write()" };

      m_gluespec.space = d(m_unitsystem);
      m_state = State::ParsedSpace;
    }

    return;
  }
  break;
  case State::ParsedSpace:
  case State::ParsedPlusDimen:
  {
    if (c == ' ')
      return;

    if (state() == State::ParsedSpace)
    {
      if (c != 'p')
        throw std::runtime_error{ "GlueParser::write()" };

      m_state = State::ParsingPlus;
      m_wordparsing = 1;
    }
    else if (state() == State::ParsedPlusDimen)
    {
      if (c != 'm')
        throw std::runtime_error{ "GlueParser::write()" };

      m_state = State::ParsingMinus;
      m_wordparsing = 1;
    }

    return;
  }
  break;
  case State::ParsingPlus:
  case State::ParsingMinus:
  {
    const char* word = state() == State::ParsingPlus ? "plus" : "minus";

    if (c == word[m_wordparsing])
    {
      m_wordparsing++;
      
      if (word[m_wordparsing] == '\0')
      {
        m_state = state() == State::ParsingPlus ? State::ParsedPlus : State::ParsedMinus;
        m_dimen_parser.reset();
      }
    }
    else
    {
      throw std::runtime_error{ "GlueParser::write()" };
    }

    return;
  }
  break;
  case State::ParsedPlus:
  case State::ParsedMinus:
  {
    if (c == ' ')
      return;

    m_state = state() == State::ParsedPlus ? State::ParsingPlusDimen : State::ParsingMinusDimen;
    m_dimen_parser.write(c);
    return;
  }
  break;
  case State::ParsingPlusDimen:
  case State::ParsingMinusDimen:
  {
    m_dimen_parser.write(c);

    if (m_dimen_parser.state() == DimenParser::State::Finished)
    {
      Dimen d = m_dimen_parser.finish();

      if(state() == State::ParsingPlusDimen)
        write(m_gluespec.stretch, m_gluespec.stretchOrder, d);
      else
        write(m_gluespec.shrink, m_gluespec.shrinkOrder, d);

      m_state = state() == State::ParsingPlusDimen ? State::ParsedPlusDimen : State::Finished;
    }

    return;
  }
  break;
  }
}

std::shared_ptr<Glue> GlueParser::finish()
{
  if (m_state == State::ParsingSpace)
  {
    Dimen d = m_dimen_parser.finish();

    if (!d.isFinite())
      throw std::runtime_error{ "GlueParser::finish()" };

    m_gluespec.space = d(m_unitsystem);
    m_state = State::ParsedSpace;
  }

  if (m_state == State::ParsedSpace)
  {
    return tex::glue(m_gluespec.space);
  }

  if (m_state == State::ParsingMinusDimen)
  {
    Dimen d = m_dimen_parser.finish();
    write(m_gluespec.shrink, m_gluespec.shrinkOrder, d);
    m_state = State::Finished;
  }

  if(m_state != State::Finished)
    throw std::runtime_error{ "GlueParser::finish()" };

  return tex::glue(m_gluespec);
}

} // namespace parsing

} // namespace tex

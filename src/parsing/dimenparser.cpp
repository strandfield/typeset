// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/parsing/dimenparser.h"

#include "tex/parsing/parsing-utils.h"

namespace tex
{

namespace parsing
{

DimenParser::DimenParser()
{

}

void DimenParser::reset()
{
  m_state = State::ParseOptionalSign;
  m_sign = '+';
  m_factor.clear();
  m_factor_is_decimal = false;
  m_unit.clear();
}

void DimenParser::write(char c)
{
  switch (state())
  {
  case State::ParseOptionalSign:
  {
    if (c == '+')
      return;

    if (c == '-')
    {
      m_sign = (m_sign == '-' ? '+' : '-');
    }
    else
    {
      if (is_digit(c))
      {
        m_state = State::ParsingFactor;
        write(c);
        return;
      }
      else
      {
        throw std::runtime_error{ "DimenParser::write()" };
      }
    }

    return;
  }
  break;
  case State::ParsingFactor:
  {
    if (is_digit(c))
    {
      m_factor.push_back(c);
    }
    else if (c == '.' || c == ',')
    {
      if(m_factor_is_decimal)
        throw std::runtime_error{ "DimenParser::write()" };

      m_factor_is_decimal = true;
      m_factor.push_back('.');
    }
    else if (is_lowercase_letter(c))
    {
      m_state = State::ParsingUnit;
      m_unit.push_back(c);
    }

    return;
  }
  break;
  case State::ParsingUnit:
  {
    if (!is_lowercase_letter(c))
    {
      if(!hasResult())
        throw std::runtime_error{ "DimenParser::write()" };

      m_state = State::Finished;
    }
    else
    {
      m_unit.push_back(c);
    }
    return;
  }
  break;
  case State::Finished:
  break;
  }
}

bool DimenParser::hasResult() const
{
  if (m_state == State::Finished)
    return true;

  Unit u = Unit::Filll;
  PhysicalUnit pu = PhysicalUnit::BigPoint;

  return state() == State::ParsingUnit && (str2phyunit(m_unit.data(), pu) || str2unit(m_unit.data(), u));
}

Dimen DimenParser::finish()
{
  float value = std::stof(m_factor) * (m_sign == '-' ? -1 : 1); 

  PhysicalUnit pu = PhysicalUnit::BigPoint;

  if (str2phyunit(m_unit.data(), pu))
    return Dimen(value, pu);

  Unit u = Unit::Filll;
  str2unit(m_unit.data(), u);
  return Dimen(value, u);
}

} // namespace parsing

} // namespace tex

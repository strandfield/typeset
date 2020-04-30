// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "fontparser.h"

FontParser::FontParser()
{

}

void FontParser::write(char c)
{
  switch (state())
  {
  case State::ParseFontName:
  {
    throw std::runtime_error{ "FontParser::write()" };
  }
  break;
  case State::ParseEqualSign:
  {
    if (c != '=')
    {
      throw std::runtime_error{ "FontParser::write()" };
    }

    m_state = State::ParseFontSpec;
  }
  break;
  case State::ParseLeftBrace:
  {
    if (c != '{')
    {
      throw std::runtime_error{ "FontParser::write()" };
    }
    
    m_state = State::ParseFontSpec;
  }
  break;
  case State::ParseFontSpec:
  {
    if (c == '}')
    {
      m_state = State::Finished;
    }
    else
    {
      m_font_spec.push_back(c);
    }
  }
  break;
  case State::Finished:
    break;
  }
}

void FontParser::write(const std::string& cs)
{
  switch (state())
  {
  case State::ParseFontName:
  {
    m_font_name = cs;
    m_state = State::ParseEqualSign;
  }
  break;
  default:
    throw std::runtime_error{ "FontParser::write()" };
  }
}

bool FontParser::isFinished() const
{
  return m_state == State::Finished;
}

const std::string& FontParser::fontname() const
{
  return m_font_name;
}

const std::string& FontParser::fontspec() const
{
  return m_font_spec;
}

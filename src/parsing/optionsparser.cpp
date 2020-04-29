// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/parsing/optionsparser.h"

#include "tex/parsing/parsing-utils.h"

#include <stdexcept>

namespace tex
{

namespace parsing
{

OptionsParser::OptionsParser()
{

}

void OptionsParser::reset()
{
  m_state = State::ParseLeftBracket;
  m_key_buffer.clear();
  m_value_buffer.clear();
  m_result.clear();
}

void OptionsParser::write(std::string& key, std::string& value)
{
  m_result.emplace_back(std::move(key), std::move(value));
}

void OptionsParser::write(char c)
{
  switch (state())
  {
  case State::ParseLeftBracket:
  {
    if (c == '[')
    {
      m_state = State::ParseKey;
    }
    else
    {
      if (c != ' ')
        throw std::runtime_error{ "OptionsParser::write()" };
    }
  }
  break;
  case State::ParseKey:
  {
    if (c == '=')
    {
      m_state = State::ParseValue;
    }
    else if (c == ',')
    {
      write(m_key_buffer, m_value_buffer);
      m_key_buffer.clear();
    }
    else if (c == ']')
    {
      if (!m_key_buffer.empty())
      {
        write(m_key_buffer, m_value_buffer);
        m_key_buffer.clear();
      }

      m_state = State::Finished;
    }
    else
    {
      m_key_buffer.push_back(c);

      if (c == ' ' && m_key_buffer.size() == 1)
        m_key_buffer.pop_back();
    }
  }
  break;
  case State::ParseValue:
  {
    if (c == ',')
    {
      write(m_key_buffer, m_value_buffer);
      m_key_buffer.clear();
      m_value_buffer.clear();
      m_state = State::ParseKey;
    }
    else if (c == ']')
    {
      write(m_key_buffer, m_value_buffer);
      m_key_buffer.clear();
      m_value_buffer.clear();
      m_state = State::Finished;
    }
    else
    {
      m_value_buffer.push_back(c);
    }
  }
  break;
  case State::Finished:
  break;
  }
}

bool OptionsParser::isFinished() const
{
  return m_state == State::Finished;
}

const std::vector<std::pair<std::string, std::string>>& OptionsParser::result() const
{
  return m_result;
}

const std::vector<std::pair<std::string, std::string>>& OptionsParser::finish()
{
  if (!isFinished())
    throw std::runtime_error{ "OptionsParser::finish()" };

  return m_result;
}

} // namespace parsing

} // namespace tex

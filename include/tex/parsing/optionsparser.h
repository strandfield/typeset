// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_OPTIONSPARSER_H
#define LIBTYPESET_OPTIONSPARSER_H

#include "tex/defs.h"

#include <string>
#include <utility>
#include <vector>

namespace tex
{

namespace parsing
{

class LIBTYPESET_API OptionsParser
{
public:

  enum class State
  {
    ParseLeftBracket,
    ParseKey,
    ParseValue,
    Finished,
  };

private:
  State m_state = State::ParseLeftBracket;
  std::string m_key_buffer;
  std::string m_value_buffer;
  std::vector<std::pair<std::string, std::string>> m_result;

public:
  OptionsParser();

  const State& state() const { return m_state; }

  void reset();

  void write(char c);

  bool isFinished() const;

  const std::vector<std::pair<std::string, std::string>>& result() const;

  const std::vector<std::pair<std::string, std::string>>& finish();

protected:
  virtual void write(std::string& key, std::string& value);

protected:
};

} // namespace parsing

} // namespace tex

#endif // LIBTYPESET_OPTIONSPARSER_H

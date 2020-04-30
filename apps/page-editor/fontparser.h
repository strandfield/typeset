// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef TYPESET_PAGEEDITOR_FONTPARSER_H
#define TYPESET_PAGEEDITOR_FONTPARSER_H

#include "tex/token.h"
#include "tex/parsing/parshapeparser.h"

#include <map>
#include <memory>
#include <vector>

class FontParser
{
public:

  enum class State
  {
    ParseFontName,
    ParseEqualSign,
    ParseLeftBrace,
    ParseFontSpec,
    Finished,
  };

private:
  State m_state = State::ParseFontName;
  std::string m_font_name;
  std::string m_font_spec;

public:
  FontParser();

  const State& state() const { return m_state; }

  void write(char c);
  void write(const std::string& cs);

  bool isFinished() const;

  const std::string& fontname() const;
  const std::string& fontspec() const;
};

#endif // TYPESET_PAGEEDITOR_FONTPARSER_H

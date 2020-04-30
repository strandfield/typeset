// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef TYPESET_PAGEEDITOR_ASSIGNMENTPROCESSOR_H
#define TYPESET_PAGEEDITOR_ASSIGNMENTPROCESSOR_H

#include "tex/font.h"
#include "tex/token.h"
#include "tex/parsing/parshapeparser.h"

#include <map>
#include <memory>
#include <vector>

class FontParser;

class TypesettingMachine;

class LIBTYPESET_API AssignmentProcessor
{
public:
  AssignmentProcessor(TypesettingMachine& m);
  ~AssignmentProcessor();

  enum class State
  {
    Main,
    /* Parshape */
    Parshape,
    /* Font */
    Font,
  };

  enum class CS
  {
    PARSHAPE,
    font,
  };

  static const std::map<std::string, CS>& csmap();
  static CS controlSequence(const std::string& name);

  void write(tex::parsing::Token& t);

  std::vector<tex::parsing::Token>& output();

protected:
  bool handleCs(const std::string& csname);
  bool changeFont(const std::string& csname);

  void write_main(tex::parsing::Token&);
  void write_parshape(tex::parsing::Token&);
  void write_font(tex::parsing::Token&);

private:
  State m_state = State::Main;
  TypesettingMachine& m_machine;
  std::map<std::string, tex::Font> m_font_map;
  std::vector<tex::parsing::Token> m_output;
  std::unique_ptr<tex::parsing::ParshapeParser> m_parshape;
  std::unique_ptr<FontParser> m_font;
};

inline std::vector<tex::parsing::Token>& AssignmentProcessor::output()
{
  return m_output;
}

#endif // TYPESET_PAGEEDITOR_ASSIGNMENTPROCESSOR_H

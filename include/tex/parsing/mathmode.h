// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_PARSING_MATHMODE_H
#define LIBTYPESET_PARSING_MATHMODE_H

#include "tex/parsing/mode.h"

#include "tex/parsing/typesetting-machine.h"
#include "tex/parsing/mathparser.h"

#include "tex/fontmetrics.h"
#include "tex/typeset.h"

namespace tex
{

namespace parsing
{

class LIBTYPESET_API MathMode : public Mode
{
public:
  MathMode(TypesettingMachine& m);
  ~MathMode() = default;

  typedef RetCode(MathMode::*Callback)();

  FontMetrics metrics() const;

  bool isInlineMath() const;

  std::map<std::string, Callback>& commands();
  void push(Callback cmd);
  void pop();

  RetCode advance() override;

  std::string& symbuf();

  MathList& mlist();

  void writeOutput();

  /* Callbacks */
  RetCode main_callback();

private:
  math::Style m_style;
  std::vector<Callback> m_callbacks;
  std::map<std::string, Callback> m_commands;
  std::string m_symbuf;
  parsing::MathParser m_parser;
};

} // namespace parsing

} // namespace tex

#endif // LIBTYPESET_PARSING_MATHMODE_H

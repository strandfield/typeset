// Copyright (C) 2019-2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef TYPESET_PAGEEDITOR_MATHMODE_H
#define TYPESET_PAGEEDITOR_MATHMODE_H

#include "mode.h"

#include "typesetting-machine.h"

#include "common/charbuffer.h"

#include "tex/parsing/mathparserfrontend.h"
#include "tex/math/math-typeset.h"

#include "tex/fontmetrics.h"
#include "tex/typeset.h"

#include <functional>

class HorizontalMode;

class MathMode : public Mode
{
public:
  MathMode(TypesettingMachine& m);
  MathMode(TypesettingMachine& m, std::function<void(MathMode&)> o_routine);
  ~MathMode() = default;

  typedef void(MathMode::*Callback)(tex::parsing::Token&&);

  enum class State
  {
    Main,
    MathShift,
  };

  enum class CS
  {

  };

  static const std::map<std::string, CS>& csmap();
  static CS controlSequence(const std::string& name);

  Kind kind() const override;
  void write(tex::parsing::Token& t) override;
  void finish() override;

  tex::MathList& mlist();

  void writeOutput();

  static void writeToHorizontalMode(HorizontalMode& output, MathMode& self);
  static void writeToVerticalMode(tex::VListBuilder& output, MathMode& self);

  std::function<void(MathMode&)> output_routine;

protected:
  void write_main(tex::parsing::Token&);
  void write_mathshift(tex::parsing::Token&);

private:
  State m_state = State::Main;
  CharBuffer m_buffer;
  tex::parsing::MathParserFrontend m_parser;
  std::array<tex::MathFont, 16> m_fonts;
};

#endif // TYPESET_PAGEEDITOR_MATHMODE_H

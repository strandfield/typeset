// Copyright (C) 2019-2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef TYPESET_PAGEEDITOR_VERTICALMODE_H
#define TYPESET_PAGEEDITOR_VERTICALMODE_H

#include "mode.h"

#include "tex/vbox.h"
#include "tex/typeset.h"

#include "tex/parsing/kernparser.h"

#include <map>
#include <vector>

class TypesettingMachine;

class LIBTYPESET_API VerticalMode : public Mode
{
public:
  VerticalMode(TypesettingMachine& m);
  ~VerticalMode() = default;

  enum class State
  {
    Main,
    MathShift,
    Kern,
  };

  Kind kind() const override;

  enum class CS
  {
    PAR,
    KERN,
  };

  static const std::map<std::string, CS>& csmap();
  static CS controlSequence(const std::string& name);

  void write(tex::parsing::Token& t) override;
  void finish() override;

  tex::VListBuilder& vlist();

protected:
  void write_main(tex::parsing::Token&);
  void write_kern(tex::parsing::Token& t);
  void write_mathshift(tex::parsing::Token&);

  void kern_callback();

private:
  State m_state = State::Main;
  std::unique_ptr<tex::parsing::KernParser> m_kern_parser;
  tex::VListBuilder m_vlist;
};

#endif // TYPESET_PAGEEDITOR_VERTICALMODE_H

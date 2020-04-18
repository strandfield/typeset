// Copyright (C) 2019-2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef TYPESET_PAGEEDITOR_VERTICALMODE_H
#define TYPESET_PAGEEDITOR_VERTICALMODE_H

#include "mode.h"

#include "tex/vbox.h"
#include "tex/typeset.h"

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
  };

  Kind kind() const override;

  enum class CS
  {

  };

  static const std::map<std::string, CS>& csmap();
  static CS controlSequence(const std::string& name);

  void write(tex::parsing::Token& t) override;
  void finish() override;

  tex::List& vlist();

protected:
  void main_callback(tex::parsing::Token&);
  void mathshift_callback(tex::parsing::Token&);

private:
  State m_state = State::Main;
  tex::List m_vlist;
};

#endif // TYPESET_PAGEEDITOR_VERTICALMODE_H

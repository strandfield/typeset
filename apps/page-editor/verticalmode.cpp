// Copyright (C) 2019-2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "verticalmode.h"

#include "typesetting-machine.h"
#include "horizontalmode.h"
#include "mathmode.h"

VerticalMode::VerticalMode(TypesettingMachine& m)
  : Mode(m)
{
}

Mode::Kind VerticalMode::kind() const
{
  return Mode::Kind::Vertical;
}

const std::map<std::string, VerticalMode::CS>& VerticalMode::csmap()
{
  static const std::map<std::string, VerticalMode::CS> map = {

  };

  return map;
}

VerticalMode::CS VerticalMode::controlSequence(const std::string& name)
{
  auto it = csmap().find(name);

  if (it == csmap().end())
    throw std::runtime_error{ "Unknown control sequence" };

  return it->second;
}

void VerticalMode::write(tex::parsing::Token& t)
{
  switch (m_state)
  {
  case State::Main:
    return main_callback(t);
  case State::MathShift:
    return mathshift_callback(t);
  default:
    break;
  }
}

void VerticalMode::finish()
{

}

tex::List& VerticalMode::vlist()
{
  return m_vlist;
}

void VerticalMode::main_callback(tex::parsing::Token& t)
{
  if (t.isControlSequence())
  {
    CS cs = controlSequence(t.controlSequence());
    
    switch (cs)
    {
    default:
      assert(false);
      break;
    }
  }
  else
  {
    if (t == tex::parsing::CharCategory::MathShift)
    {
      m_state = State::MathShift;
    }
    else
    {
      machine().enter<HorizontalMode>();
      machine().insert(std::move(t));
    }
  }
}

void VerticalMode::mathshift_callback(tex::parsing::Token& t)
{
  m_state = State::Main;

  if (t == tex::parsing::CharCategory::MathShift)
  {
    machine().enter<MathMode>();
  }
  else
  {
    machine().enter<HorizontalMode>();

    machine().insert(std::move(t));

    tex::parsing::CharacterToken ctok;
    ctok.value = '$';
    ctok.category = tex::parsing::CharCategory::MathShift;
    machine().insert(tex::parsing::Token{ ctok });
  }
}

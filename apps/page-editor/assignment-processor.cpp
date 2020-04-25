// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "assignment-processor.h"

#include "typesetting-machine.h"

AssignmentProcessor::AssignmentProcessor(TypesettingMachine& m)
  : m_machine(m)
{

}

const std::map<std::string, AssignmentProcessor::CS>& AssignmentProcessor::csmap()
{
  static const std::map<std::string, AssignmentProcessor::CS> map = {
    {"parshape", CS::PARSHAPE},
  };

  return map;
}

AssignmentProcessor::CS AssignmentProcessor::controlSequence(const std::string& name)
{
  auto it = csmap().find(name);

  if (it == csmap().end())
    throw std::runtime_error{ "Unknown control sequence" };

  return it->second;
}

void AssignmentProcessor::write(tex::parsing::Token& t)
{
  switch (m_state)
  {
  case State::Main:
    return write_main(t);
  case State::Parshape:
    return write_parshape(t);
  default:
    assert(false);
    break;
  }
}

void AssignmentProcessor::write_main(tex::parsing::Token& t)
{
  if (t.isControlSequence())
  {
    auto it = csmap().find(t.controlSequence());

    if (it == csmap().end())
    {
      m_output.push_back(std::move(t));
      return;
    }

    CS cs = it->second;
    
    switch (cs)
    {
    case CS::PARSHAPE:
    {
      tex::UnitSystem us = m_machine.unitSystem();
      m_parshape.reset(new tex::parsing::ParshapeParser(us));
      m_state = State::Parshape;
    }
      break;
    default:
      assert(false);
      break;
    }
  }
  else
  {
    m_output.push_back(std::move(t));
  }
}

void AssignmentProcessor::write_parshape(tex::parsing::Token& t)
{
  if (t.isCharacterToken())
  {
    m_parshape->write(t.characterToken().value);
  }

  if (m_parshape->isFinished() || t.isControlSequence())
  {
    tex::Parshape ps = m_parshape->finish();
    m_machine.memory().parshape = ps;
    m_parshape.reset();
    m_state = State::Main;

    if (t.isControlSequence())
      write(t);
  }
}

// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "assignment-processor.h"

#include "fontparser.h"
#include "typesetting-machine.h"

AssignmentProcessor::AssignmentProcessor(TypesettingMachine& m)
  : m_machine(m)
{

}

AssignmentProcessor::~AssignmentProcessor()
{

}

const std::map<std::string, AssignmentProcessor::CS>& AssignmentProcessor::csmap()
{
  static const std::map<std::string, AssignmentProcessor::CS> map = {
    {"parshape", CS::PARSHAPE},
    {"font", CS::font},
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
  case State::Font:
    return write_font(t);
  default:
    assert(false);
    break;
  }
}

bool AssignmentProcessor::handleCs(const std::string& csname)
{
  auto it = csmap().find(csname);

  if (it == csmap().end())
    return false;

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
  case CS::font:
  {
    m_font.reset(new FontParser());
    m_state = State::Font;
  }
  break;
  default:
    assert(false);
    break;
  }

  return true;
}

bool AssignmentProcessor::changeFont(const std::string& csname)
{
  auto it = m_font_map.find(csname);

  if (it == m_font_map.end())
    return false;

  m_machine.memory().font = it->second;

  return true;
}

void AssignmentProcessor::write_main(tex::parsing::Token& t)
{
  if (t.isControlSequence())
  {
    if (handleCs(t.controlSequence()))
      return;

    if (changeFont(t.controlSequence()))
      return;

    m_output.push_back(std::move(t));
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

void AssignmentProcessor::write_font(tex::parsing::Token& t)
{
  if (t.isControlSequence())
  {
    m_font->write(t.controlSequence());
  }
  else
  {
    m_font->write(t.characterToken().value);
  }

  if (m_font->isFinished())
  {
    tex::Font f = m_machine.typesetEngine()->loadFont(m_font->fontname(), m_font->fontspec());
    m_font_map[m_font->fontname()] = f;
    m_state = State::Main;
  }
}

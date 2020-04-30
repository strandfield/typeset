// Copyright (C) 2019-2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "verticalmode.h"

#include "typesetting-machine.h"
#include "horizontalmode.h"
#include "mathmode.h"

VerticalMode::VerticalMode(TypesettingMachine& m)
  : Mode(m),
    m_vlist{ m.memory().baselineskip, m.memory().lineskip }
{
  m_vlist.lineskiplimit = m.memory().lineskiplimit;
}

Mode::Kind VerticalMode::kind() const
{
  return Mode::Kind::Vertical;
}

const std::map<std::string, VerticalMode::CS>& VerticalMode::csmap()
{
  static const std::map<std::string, VerticalMode::CS> map = {
    {"par", CS::PAR},
    {"kern", CS::KERN}
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
    return write_main(t);
  case State::MathShift:
    return write_mathshift(t);
  case State::Kern:
    return write_kern(t);
  default:
    break;
  }
}

void VerticalMode::finish()
{

}

tex::VListBuilder& VerticalMode::vlist()
{
  return m_vlist;
}

void VerticalMode::write_main(tex::parsing::Token& t)
{
  if (t.isControlSequence())
  {
    CS cs = controlSequence(t.controlSequence());
    
    switch (cs)
    {
    case CS::PAR:
      break;
    case CS::KERN:
      kern_callback();
      break;
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
      machine().enter<HorizontalMode>([this](HorizontalMode& hm) {
        HorizontalMode::writeToVerticalMode(vlist(), hm);
        });

      machine().insert(std::move(t));
    }
  }
}

void VerticalMode::write_kern(tex::parsing::Token& t)
{
  if (t.isControlSequence())
  {
    m_vlist.result.push_back(m_kern_parser->finish());
    m_kern_parser.reset();
    m_state = State::Main;
    write(t);
    return;
  }

  m_kern_parser->write(t.characterToken().value);

  if (m_kern_parser->isFinished())
  {
    m_vlist.result.push_back(m_kern_parser->finish());
    m_kern_parser.reset();
    m_state = State::Main;
  }
}

void VerticalMode::write_mathshift(tex::parsing::Token& t)
{
  m_state = State::Main;

  if (t == tex::parsing::CharCategory::MathShift)
  {
    machine().enter<MathMode>([this](MathMode& mm) {
      MathMode::writeToVerticalMode(vlist(), mm);
      });
  }
  else
  {
    machine().enter<HorizontalMode>();

    machine().enter<HorizontalMode>([this](HorizontalMode& hm) {
      HorizontalMode::writeToVerticalMode(vlist(), hm);
      });

    machine().insert(std::move(t));

    tex::parsing::CharacterToken ctok;
    ctok.value = '$';
    ctok.category = tex::parsing::CharCategory::MathShift;
    machine().insert(tex::parsing::Token{ ctok });
  }
}

void VerticalMode::kern_callback()
{
  tex::UnitSystem us = machine().unitSystem();
  m_kern_parser.reset(new tex::parsing::KernParser(us));
  m_state = State::Kern;
}


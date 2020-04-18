// Copyright (C) 2019-2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "horizontalmode.h"

#include "typesetting-machine.h"
#include "mathmode.h"
#include "verticalmode.h"

#include "tex/linebreaks.h"

#include <cassert>

HorizontalMode::HorizontalMode(TypesettingMachine& m)
  : Mode(m)
{

}

tex::FontMetrics HorizontalMode::metrics() const
{
  return tex::FontMetrics{ machine().memory().font, machine().typesetEngine()->metrics() };
}

void HorizontalMode::write_main(tex::parsing::Token& t)
{
  if (t.isControlSequence())
  {
    CS cs = controlSequence(t.controlSequence());

    switch (cs)
    {
    case CS::PAR:
      return par_callback(t);
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
      return;
    }

    tex::parsing::CharacterToken ctok = t.characterToken();

    if (ctok.category == tex::parsing::CharCategory::Letter || ctok.category == tex::parsing::CharCategory::Other)
    {
      m_buffer.write(ctok.value);

      if (m_buffer.ready())
      {
        tex::Character c = m_buffer.read();
        auto box = machine().typesetEngine()->typeset(c, currentFont());
        m_hlist.push_back(box);
      }
    }
    else
    {
      if (ctok.category == tex::parsing::CharCategory::Space)
      {
        tex::FontMetrics m = metrics();
        auto g = glue(m.interwordSpace(), tex::Stretch{ m.interwordStretch() }, tex::Shrink{ m.interwordShrink() });
        m_hlist.push_back(g);
      }
      else if (ctok.category == tex::parsing::CharCategory::GroupBegin)
      {
        machine().beginGroup();
      }
      else if (ctok.category == tex::parsing::CharCategory::GroupEnd)
      {
        machine().endGroup();
      }
    }

  }
}

void HorizontalMode::write_mathshift(tex::parsing::Token& t)
{
  m_state = State::Main;

  if (t == tex::parsing::CharCategory::MathShift)
  {
    writeOutput();

    machine().insert(tex::parsing::Token{ t });
    machine().insert(std::move(t));
  }
  else
  {
    machine().enter<MathMode>();
    machine().insert(std::move(t));
  }
}

void HorizontalMode::par_callback(tex::parsing::Token& t)
{
  writeOutput();
}

Mode::Kind HorizontalMode::kind() const
{
  return Mode::Kind::Horizontal;
}

const std::map<std::string, HorizontalMode::CS>& HorizontalMode::csmap()
{
  static const std::map<std::string, HorizontalMode::CS> map = {
    {"par", CS::PAR},
  };

  return map;
}

HorizontalMode::CS HorizontalMode::controlSequence(const std::string& name)
{
  auto it = csmap().find(name);

  if (it == csmap().end())
    throw std::runtime_error{ "Unknown control sequence" };

  return it->second;
}

void HorizontalMode::write(tex::parsing::Token& t)
{
  switch (m_state)
  {
  case State::Main:
    return write_main(t);
  case State::MathShift:
    return write_mathshift(t);
  default:
    break;
  }
}

void HorizontalMode::finish()
{
  writeOutput();
}

tex::Font HorizontalMode::currentFont() const
{
  return machine().memory().font;
}

tex::List& HorizontalMode::hlist()
{
  return m_hlist;
}


void HorizontalMode::writeOutput()
{
  VerticalMode* vm = static_cast<VerticalMode*>(&parent());

  tex::Paragraph p;
  p.parshape() = machine().memory().parshape;
  p.prepare(hlist());
  tex::List l = p.create(hlist());

  vm->vlist().insert(vm->vlist().end(), l.begin(), l.end());

  machine().leaveCurrentMode();
}

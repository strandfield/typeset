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
  : Mode(m),
    m_hlist(m.typesetEngine(), m.memory().font)
{
  m_is_restricted = parent().kind() == Mode::Kind::Horizontal;
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
      return par_callback();
    case CS::KERN:
      return kern_callback();
    case CS::HBOX:
      return hbox_callback();
    case CS::LOWER:
      return lower_callback();
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
        m_hlist.push_back(c);
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
        beginGroup();
      }
      else if (ctok.category == tex::parsing::CharCategory::GroupEnd)
      {
        endGroup();
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

void HorizontalMode::write_kern(tex::parsing::Token& t)
{
  if (t.isControlSequence())
  {
    m_hlist.push_back(m_kern_parser->finish());
    m_kern_parser.reset();
    m_state = State::Main;
    write(t);
    return;
  }

  m_kern_parser->write(t.characterToken().value);

  if (m_kern_parser->isFinished())
  {
    m_hlist.push_back(m_kern_parser->finish());
    m_kern_parser.reset();
    m_state = State::Main;
  }
}

void HorizontalMode::write_lower(tex::parsing::Token& t)
{
  if(t.isCharacterToken())
    m_dimen_parser->write(t.characterToken().value);

  if (t.isControlSequence() || m_dimen_parser->state() == tex::parsing::DimenParser::State::Finished)
  {
    tex::Dimen d = m_dimen_parser->finish();
    tex::UnitSystem us = machine().unitSystem();
    m_lower = d(us);
    m_dimen_parser.reset();
    m_state = State::Main;

    if (t.isControlSequence())
      write(t);
  }
}

void HorizontalMode::beginGroup()
{
  Mode::beginGroup();
  machine().beginGroup();
}

void HorizontalMode::endGroup()
{
  size_t depth = Mode::endGroup();
  machine().endGroup();

  if (m_is_restricted && depth == 0)
    writeOutput();
}

void HorizontalMode::par_callback()
{
  writeOutput();
}

void HorizontalMode::kern_callback()
{
  tex::UnitSystem us = machine().unitSystem();
  m_kern_parser.reset(new tex::parsing::KernParser(us));
  m_state = State::Kern;
}

void HorizontalMode::hbox_callback()
{
  machine().enter<HorizontalMode>();
}

void HorizontalMode::lower_callback()
{
  m_dimen_parser.reset(new tex::parsing::DimenParser());
  m_state = State::Lower;
}

Mode::Kind HorizontalMode::kind() const
{
  return Mode::Kind::Horizontal;
}

const std::map<std::string, HorizontalMode::CS>& HorizontalMode::csmap()
{
  static const std::map<std::string, HorizontalMode::CS> map = {
    {"par", CS::PAR},
    {"kern", CS::KERN},
    {"hbox", CS::HBOX},
    {"lower", CS::LOWER},
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
  case State::Kern:
    return write_kern(t);
  case State::Lower:
    return write_lower(t);
  default:
    assert(false);
    break;
  }
}

void HorizontalMode::write(std::shared_ptr<tex::ListBox> box)
{
  if (m_lower != 0.f)
  {
    box->setShiftAmount(box->shiftAmount() + m_lower);
    m_lower = 0.f;
  }

  hlist().push_back(box);
}

void HorizontalMode::finish()
{
  writeOutput();
}

tex::Font HorizontalMode::currentFont() const
{
  return machine().memory().font;
}

tex::HListBuilder& HorizontalMode::hlist()
{
  return m_hlist;
}


void HorizontalMode::writeOutput()
{
  if (parent().kind() == Mode::Kind::Vertical)
  {
    VerticalMode* vm = static_cast<VerticalMode*>(&parent());

    tex::Paragraph p;
    p.prevdepth = vm->vlist().prevdepth;
    p.baselineskip = machine().memory().baselineskip;
    p.lineskip = machine().memory().lineskip;
    p.lineskiplimit = machine().memory().lineskiplimit;
    p.hsize = machine().memory().hsize;
    p.parshape = machine().memory().parshape;
    p.prepare(hlist().result);
    tex::List l = p.create(hlist().result);

    vm->vlist().result.insert(vm->vlist().result.end(), l.cbegin(), l.cend());
    vm->vlist().prevdepth = p.prevdepth;
  }
  else
  {
    HorizontalMode* hm = static_cast<HorizontalMode*>(&parent());

    auto b = tex::hbox(std::move(hlist().result));
    hm->write(b);
  }

  machine().leaveCurrentMode();
}

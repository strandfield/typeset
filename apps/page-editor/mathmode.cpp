// Copyright (C) 2019-2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "mathmode.h"

#include "typesetting-machine.h"
#include "horizontalmode.h"
#include "verticalmode.h"

#include "tex/math/fraction.h"
#include "tex/math/math-typeset.h"
#include "tex/math/mathlist.h"
#include "tex/math/stylechange.h"

#include "tex/unicode.h"

#include <cassert>

MathMode::MathMode(TypesettingMachine& m)
  : Mode(m)
{
  // @TODO: handle fonts correctly
  {
    for (size_t i(0); i < 16; ++i)
    {
      m_fonts[i].textfont = tex::Font(3 * i );
      m_fonts[i].scriptfont = tex::Font(3 * i + 1);
      m_fonts[i].scriptscriptfont = tex::Font(3 * i + 2);
    }
  }

}

Mode::Kind MathMode::kind() const
{
  return Mode::Kind::Math;
}


const std::map<std::string, MathMode::CS>& MathMode::csmap()
{
  static const std::map<std::string, MathMode::CS> map = {

  };

  return map;
}

MathMode::CS MathMode::controlSequence(const std::string& name)
{
  auto it = csmap().find(name);

  if (it == csmap().end())
    throw std::runtime_error{ "Unknown control sequence" };

  return it->second;
}

void MathMode::write(tex::parsing::Token& t)
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

void MathMode::finish()
{
  m_parser.finish();
  writeOutput();
}

tex::MathList& MathMode::mlist()
{
  return m_parser.output();
}

void MathMode::writeOutput()
{
  if (parent().kind() == Mode::Kind::Vertical)
  {
    auto& vm = parent().as<VerticalMode>();

    tex::MathTypesetter mt{ machine().typesetEngine() };
    mt.setFonts(m_fonts);

    tex::List hlist = mt.mlist2hlist(mlist(), tex::math::Style::D);

    std::shared_ptr<tex::HBox> box = tex::hbox(std::move(hlist));

    vm.vlist().push_back(box);
  }
  else
  {
    auto& hm = parent().as<HorizontalMode>();

    tex::MathTypesetter mt{ machine().typesetEngine() };
    mt.setFonts(m_fonts);

    tex::List hlist = mt.mlist2hlist(mlist(), tex::math::Style::T);

    hm.hlist().result.insert(hm.hlist().result.end(), hlist.begin(), hlist.end());
    hm.hlist().spacefactor = 1000;
  }

  machine().leaveCurrentMode();
}

void MathMode::write_main(tex::parsing::Token& t)
{
  if (t.isControlSequence())
  {
    auto it = csmap().find(t.controlSequence());

    if (it != csmap().end())
    {
      CS cs = it->second;

      switch (cs)
      {
      default:
        assert(false);
        break;
      }
    }
    else
    {
      m_parser.writeControlSequence(t.controlSequence());
    }
  }
  else
  {
    if (t == tex::parsing::CharCategory::MathShift)
    {
      if (parent().kind() == Mode::Kind::Horizontal)
      {
        m_parser.finish();
        writeOutput();
      }
      else
      {
        m_state = State::MathShift;
      }

      return;
    }

    tex::parsing::CharacterToken ctok = t.characterToken();

    if (ctok.category == tex::parsing::CharCategory::Letter || ctok.category == tex::parsing::CharCategory::Other)
    {
      m_buffer.write(ctok.value);

      if (m_buffer.ready())
      {
        m_parser.writeSymbol(m_buffer.read());
      }
    }
    else if (ctok.category == tex::parsing::CharCategory::Subscript)
    {
      m_parser.beginSubscript();
    }
    else if (ctok.category == tex::parsing::CharCategory::Superscript)
    {
      m_parser.beginSuperscript();
    }
    else if (ctok.category == tex::parsing::CharCategory::GroupBegin)
    {
      machine().beginGroup();
      m_parser.beginMathList();
    }
    else if (ctok.category == tex::parsing::CharCategory::GroupEnd)
    {
      m_parser.endMathList();
      machine().endGroup();
    }
  }
}

void MathMode::write_mathshift(tex::parsing::Token& t)
{
  if (t != tex::parsing::CharCategory::MathShift)
    throw std::runtime_error{ "Unexpected single '$' in display math mode" };

  m_parser.finish();
  writeOutput();
}

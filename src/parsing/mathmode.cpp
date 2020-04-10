// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/parsing/mathmode.h"

#include "tex/parsing/typesetting-machine.h"
#include "tex/parsing/horizontalmode.h"
#include "tex/parsing/verticalmode.h"

#include "tex/math/fontchange.h"
#include "tex/math/fraction.h"
#include "tex/math/math-typeset.h"
#include "tex/math/mathlist.h"
#include "tex/math/stylechange.h"

#include "tex/unicode.h"

#include <cassert>

namespace tex
{

namespace parsing
{

MathMode::MathMode(TypesettingMachine& m)
  : Mode(m),
  m_style(math::Style::D)
{
  if (parent() && dynamic_cast<HorizontalMode*>(parent()))
    m_style = math::Style::T;

  push(&MathMode::main_callback);

  m_commands = {};

  parsing::read(machine().preprocessor().output(), m_style == math::Style::T ? 1 : 2);
}

FontMetrics MathMode::metrics() const
{
  const Options& opts = machine().options();
  return FontMetrics{ opts.font(), machine().typesetEngine()->metrics() };
}

bool MathMode::isInlineMath() const
{
  return m_style == math::Style::T;
}

std::map<std::string, MathMode::Callback>& MathMode::commands()
{
  return m_commands;
}

void MathMode::push(Callback cmd)
{
  m_callbacks.push_back(cmd);
}

void MathMode::pop()
{
  m_callbacks.pop_back();
}

bool MathMode::write(Token&& t)
{
  Callback f = m_callbacks.back();
  (this->*f)(std::move(t));
  return done();
}

std::string& MathMode::symbuf()
{
  return m_symbuf;
}

MathList& MathMode::mlist()
{
  return m_parser.output();
}

void MathMode::writeOutput()
{
  VerticalMode* vm = dynamic_cast<VerticalMode*>(parent());

  if (vm)
  {
    Options opts = machine().options().withStyle(math::Style::D)
      .withFont(Font::MathItalic);
    List hlist = tex::mlist_to_hlist(std::move(mlist()), opts);

    std::shared_ptr<HBox> box = tex::hbox(std::move(hlist));

    vm->vlist().insert(vm->vlist().end(), box);
  }
  else
  {
    HorizontalMode* hm = dynamic_cast<HorizontalMode*>(parent());

    Options opts = machine().options().withStyle(math::Style::T)
      .withFont(Font::MathItalic);
    List hlist = tex::mlist_to_hlist(std::move(mlist()), opts);

    hm->hlist().insert(hm->hlist().end(), hlist.begin(), hlist.end());
  }

  setDone();
}

void MathMode::main_callback(Token&& t)
{
  if (t.isControlSequence())
  {
    auto it = commands().find(t.controlSequence());

    if (it == commands().end())
    {
      try
      {
        m_parser.writeControlSequence(t.controlSequence());
      }
      catch (...)
      {
        throw std::runtime_error{ "No such control sequence in Math mode" };
      }
    }
    else
    {
      Callback f = it->second;
      (this->*f)(std::move(t));
    }
  }
  else
  {
    if (t == CharCategory::MathShift)
    {
      if (isInlineMath())
      {
        m_parser.finish();
        writeOutput();
      }
      else
      {
        m_callbacks.push_back(&MathMode::mathshift_callback);
      }

      return;
    }

    CharacterToken ctok = t.characterToken();

    if (ctok.category == CharCategory::Letter || ctok.category == CharCategory::Other)
    {
      symbuf().push_back(ctok.value);

      if (is_utf8_char(symbuf()))
      {
        auto it = symbuf().begin();
        m_parser.writeSymbol(read_utf8_char(it));
        symbuf().clear();
      }
    }
    else if (ctok.category == CharCategory::Subscript)
    {
      m_parser.beginSubscript();
    }
    else if (ctok.category == CharCategory::Superscript)
    {
      m_parser.beginSuperscript();
    }
    else if (ctok.category == CharCategory::GroupBegin)
    {
      m_parser.beginMathList();
    }
    else if (ctok.category == CharCategory::GroupEnd)
    {
      m_parser.endMathList();
    }
  }
}

void MathMode::mathshift_callback(Token&& t)
{
  if (t != CharCategory::MathShift)
    throw std::runtime_error{ "Unexpected single '$' in display math mode" };

  m_parser.finish();
  writeOutput();

  m_callbacks.clear();
}

} // namespace parsing

} // namespace tex

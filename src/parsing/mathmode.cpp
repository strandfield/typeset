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

  read(tokens(), m_style == math::Style::T ? 1 : 2);
}

FontMetrics MathMode::metrics() const
{
  const Options& opts = machine().options();
  return FontMetrics{ opts.font(), opts.fontSize(), machine().typesetEngine()->metrics() };
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

RetCode MathMode::advance()
{
  Callback f = m_callbacks.back();
  return (this->*f)();
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
}

RetCode MathMode::main_callback()
{
  std::vector<Token>& toks = tokens();

  if (peek(toks).isControlSequence())
  {
    auto it = commands().find(toks.front().controlSequence());

    if (it == commands().end())
    {
      try
      {
        Token cstok = read(toks);
        m_parser.writeControlSequence(MathParser::cs(cstok.controlSequence()));
        return RetCode::Yield;
      }
      catch (...)
      {
        throw std::runtime_error{ "No such control sequence in Math mode" };
      }
    }
    else
    {
      Callback f = it->second;
      return (this->*f)();
    }
  }
  else
  {
    if (peek(toks) == CharCategory::MathShift)
    {
      if (isInlineMath())
      {
        read(toks);
        m_parser.finish();
        writeOutput();
        return RetCode::Return;
      }

      if (toks.size() == 1)
      {
        return RetCode::Await; // need more tokens
      }

      if (peek(toks, 1) == CharCategory::MathShift)
      {
        read(toks, 2);
        m_parser.finish();
        writeOutput();
        return RetCode::Return;
      }
      else
      {
        read(toks);
        throw std::runtime_error{ "Unexpected '$' in display math mode" };
      }

      return RetCode::Yield;
    }

    CharacterToken ctok = read(toks).characterToken();

    if (ctok.category == CharCategory::Letter || ctok.category == CharCategory::Other)
    {
      symbuf().push_back(ctok.value);

      if (is_utf8_char(symbuf()))
      {
        m_parser.writeSymbol(symbuf());
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
   
    return RetCode::Yield;
  }
}

} // namespace parsing

} // namespace tex

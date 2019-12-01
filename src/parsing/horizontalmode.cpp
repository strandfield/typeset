// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/parsing/horizontalmode.h"

#include "tex/parsing/typesetting-machine.h"
#include "tex/parsing/mathmode.h"
#include "tex/parsing/verticalmode.h"

#include "tex/linebreaks.h"

#include <cassert>

namespace tex
{

namespace parsing
{

HorizontalMode::HorizontalMode(TypesettingMachine& m)
  : Mode(m),
  m_typesetter{ m.typesetEngine() }
{
  push(&HorizontalMode::main_callback);

  m_commands["par"] = &HorizontalMode::par_callback;
}

FontMetrics HorizontalMode::metrics() const
{
  const Options& opts = machine().options();
  return FontMetrics{ opts.font(), opts.fontSize(), machine().typesetEngine()->metrics() };
}

RetCode HorizontalMode::main_callback(HorizontalMode& self)
{
  std::vector<Token>& tokens = self.tokens();

  if (peek(tokens).isControlSequence())
  {
    auto it = self.commands().find(tokens.front().controlSequence());

    if (it == self.commands().end())
    {
      throw std::runtime_error{ "No such control sequence in Horizontal mode" };
    }

    if (self.typesetter().ready())
    {
      self.prepareTypesetter();
      self.hlist().push_back(self.typesetter().print());
    }

    return it->second(self);
  }
  else
  {
    if (peek(tokens) == CharCategory::MathShift)
    {
      if (tokens.size() == 1)
      {
        return RetCode::Await; // need more tokens
      }

      if (self.typesetter().ready())
      {
        self.prepareTypesetter();
        self.hlist().push_back(self.typesetter().print());
      }

      if (peek(tokens, 1) == CharCategory::MathShift)
      {
        self.writeOutput();
        return RetCode::Return;
      }
      else
      {
        self.machine().enter<MathMode>();
      }

      return RetCode::Yield;
    }

    CharacterToken ctok = read(tokens).characterToken();

    if (ctok.category == CharCategory::Letter || ctok.category == CharCategory::Other)
    {
      self.typesetter().buffer().push_back(ctok.value);

      if (self.typesetter().ready())
      {
        if (self.machine().typesetIndividualCharacters())
        {
          self.prepareTypesetter();
          self.hlist().push_back(self.typesetter().print());
        }
      }
    }
    else
    {
      if (self.typesetter().ready())
      {
        self.prepareTypesetter();
        self.hlist().push_back(self.typesetter().print());
      }

      if (ctok.category == CharCategory::Space)
      {
        FontMetrics m = self.metrics();
        auto g = glue(m.interwordSpace(), tex::Stretch{ m.interwordStretch() }, tex::Shrink{ m.interwordShrink() });
        self.hlist().push_back(g);
      }
      else if (ctok.category == CharCategory::GroupBegin)
      {
        self.machine().beginGroup();
      }
      else if (ctok.category == CharCategory::GroupEnd)
      {
        self.machine().endGroup();
      }
    }

    return RetCode::Yield;
  }
}

RetCode HorizontalMode::par_callback(HorizontalMode& self)
{
  self.writeOutput();
  parsing::read(self.tokens());
  return RetCode::Return;
}

void HorizontalMode::push(Callback cmd)
{
  m_callbacks.push_back(cmd);
}

void HorizontalMode::pop()
{
  m_callbacks.pop_back();
}

RetCode HorizontalMode::advance()
{
  return m_callbacks.back()(*this);
}

List& HorizontalMode::hlist()
{
  return m_hlist;
}

void HorizontalMode::prepareTypesetter()
{
  m_typesetter.setFont(machine().options().font());
  m_typesetter.setFontSize(machine().options().fontSize());
}

void HorizontalMode::writeOutput()
{
  VerticalMode* vm = static_cast<VerticalMode*>(parent());

  Paragraph p;
  p.parshape() = { float(vm->pageWidth()) };
  p.prepare(hlist());
  List l =p.create(hlist());

  vm->vlist().insert(vm->vlist().end(), l.begin(), l.end());
}

} // namespace parsing

} // namespace tex

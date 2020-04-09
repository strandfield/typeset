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
  return FontMetrics{ opts.font(), machine().typesetEngine()->metrics() };
}

void HorizontalMode::main_callback(HorizontalMode& self, Token&& t)
{
  if (t.isControlSequence())
  {
    auto it = self.commands().find(t.controlSequence());

    if (it == self.commands().end())
    {
      throw std::runtime_error{ "No such control sequence in Horizontal mode" };
    }

    if (self.typesetter().ready())
    {
      self.prepareTypesetter();
      self.hlist().push_back(self.typesetter().print());
    }

    return it->second(self, std::move(t));
  }
  else
  {
    if (self.typesetter().ready())
    {
      self.prepareTypesetter();
      self.hlist().push_back(self.typesetter().print());
    }

    if (t == CharCategory::MathShift)
    {
      self.push(&HorizontalMode::mathshift_callback);
      return;
    }

    CharacterToken ctok = t.characterToken();

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
  }
}

void HorizontalMode::mathshift_callback(HorizontalMode& self, Token&& t)
{
  self.pop();

  std::vector<Token>& preproc_output = self.machine().preprocessor().output();
  preproc_output.insert(preproc_output.begin(), t);

  if (t == CharCategory::MathShift)
  {
    preproc_output.insert(preproc_output.begin(), t);
    self.writeOutput();
  }
  else
  {
    self.machine().enter<MathMode>();
  }
}

void HorizontalMode::par_callback(HorizontalMode& self, Token&& t)
{
  self.writeOutput();
}

void HorizontalMode::push(Callback cmd)
{
  m_callbacks.push_back(cmd);
}

void HorizontalMode::pop()
{
  m_callbacks.pop_back();
}

bool HorizontalMode::write(Token&& t)
{
  m_callbacks.back()(*this, std::move(t));
  return done();
}

List& HorizontalMode::hlist()
{
  return m_hlist;
}

void HorizontalMode::prepareTypesetter()
{
  m_typesetter.setFont(machine().options().font());
}

void HorizontalMode::writeOutput()
{
  VerticalMode* vm = static_cast<VerticalMode*>(parent());

  Paragraph p;
  p.parshape() = { float(vm->pageWidth()) };
  p.prepare(hlist());
  List l =p.create(hlist());

  vm->vlist().insert(vm->vlist().end(), l.begin(), l.end());

  setDone();
}

} // namespace parsing

} // namespace tex

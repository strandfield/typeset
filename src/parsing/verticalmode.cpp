// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/parsing/verticalmode.h"

#include "tex/parsing/typesetting-machine.h"
#include "tex/parsing/horizontalmode.h"
#include "tex/parsing/mathmode.h"

namespace tex
{

namespace parsing
{

VerticalMode::VerticalMode(TypesettingMachine& m)
  : Mode(m)
{
  m_callbacks.push_back(&VerticalMode::main_callback);
}

bool VerticalMode::write(Token&& t)
{
  m_callbacks.back()(*this, std::move(t));
  return done();
}

List& VerticalMode::vlist()
{
  return m_vlist;
}

void VerticalMode::push(Callback cmd)
{
  m_callbacks.push_back(cmd);
}

void VerticalMode::pop()
{
  m_callbacks.pop_back();
}

void VerticalMode::main_callback(VerticalMode& self, Token&& t)
{
  if (t.isControlSequence())
  {
    auto it = self.commands().find(t.controlSequence());

    if (it == self.commands().end())
    {
      throw std::runtime_error{ "No such control sequence in Vertical mode" };
    }

    it->second(self, std::move(t));
  }
  else
  {
    if (t == CharCategory::MathShift)
    {
      self.push(&VerticalMode::mathshift_callback);
    }
    else
    {
      std::vector<Token>& preproc_output = self.machine().preprocessor().output();
      preproc_output.insert(preproc_output.begin(), t);

      self.machine().enter<HorizontalMode>();
    }
  }
}

void VerticalMode::mathshift_callback(VerticalMode& self, Token&& t)
{
  self.pop();

  std::vector<Token>& preproc_output = self.machine().preprocessor().output();
  preproc_output.insert(preproc_output.begin(), t);

  if (t == CharCategory::MathShift)
  {
    preproc_output.insert(preproc_output.begin(), t);
    self.machine().enter<MathMode>();
  }
  else
  {
    self.machine().enter<HorizontalMode>();
  }
}

} // namespace parsing

} // namespace tex

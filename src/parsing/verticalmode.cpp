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

RetCode VerticalMode::advance()
{
  return m_callbacks.back()(*this);
}

List& VerticalMode::vlist()
{
  return m_vlist;
}

RetCode VerticalMode::main_callback(VerticalMode& self)
{
  std::vector<Token>& tokens = self.tokens();

  if (peek(tokens).isControlSequence())
  {
    auto it = self.commands().find(tokens.front().controlSequence());

    if (it == self.commands().end())
    {
      throw std::runtime_error{ "No such control sequence in Vertical mode" };
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

      if (peek(tokens, 1) == CharCategory::MathShift)
      {
        self.machine().enter<MathMode>();
      }
      else
      {
        self.machine().enter<HorizontalMode>();
      }

      return RetCode::Yield;
    }
    else
    {
      self.machine().enter<HorizontalMode>();

      return RetCode::Yield;
    }
  }
}

} // namespace parsing

} // namespace tex

// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/parsing/mode.h"

#include "tex/parsing/machine.h"

namespace tex
{

namespace parsing
{

Mode::Mode(Machine& m)
  : m_machine(m)
{
  m_parent = m.modes().empty() ? nullptr : m.modes().back().get();
}

Mode* Mode::parent() const
{
  return m_parent;
}

std::vector<Token>& Mode::tokens()
{
  return m_machine.preprocessor().output();
}

} // namespace parsing

} // namespace tex

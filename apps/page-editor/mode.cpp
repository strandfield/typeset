// Copyright (C) 2019-2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "mode.h"

#include "typesetting-machine.h"

Mode::Mode(TypesettingMachine& m)
  : m_machine(m)
{
  m_parent = m.modes().empty() ? nullptr : m.modes().back().get();
}

Mode& Mode::parent() const
{
  return *m_parent;
}

// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/parsing/mathmode.h"

#include "tex/parsing/typesetting-machine.h"

namespace tex
{

namespace parsing
{

MathMode::MathMode(TypesettingMachine& m)
  : Mode(m)
{

}

TypesettingMachine& MathMode::machine() const
{
  return static_cast<TypesettingMachine&>(Mode::machine());
}

FontMetrics MathMode::metrics() const
{
  const Options& opts = machine().options();
  return FontMetrics{ opts.font(), opts.fontSize(), machine().typesetEngine()->metrics() };
}

RetCode MathMode::advance()
{
  throw std::runtime_error{ "Not implemented" };
}

List& MathMode::mlist()
{
  return m_mlist;
}

} // namespace parsing

} // namespace tex

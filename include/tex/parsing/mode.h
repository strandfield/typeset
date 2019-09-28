// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_PARSING_MODE_H
#define LIBTYPESET_PARSING_MODE_H

#include "tex/token.h"

#include "tex/parsing/retcode.h"

#include <vector>

namespace tex
{

namespace parsing
{

class Machine;

class LIBTYPESET_API Mode
{
public:
  Mode(Machine& m);
  virtual ~Mode() = default;

  Machine& machine() const;
  Mode* parent() const;
  std::vector<Token>& tokens();

  virtual RetCode advance() = 0;

private:
  Machine& m_machine;
  Mode* m_parent;
};

} // namespace parsing

} // namespace tex

namespace tex
{

namespace parsing
{

inline Machine& Mode::machine() const
{
  return m_machine;
}

} // namespace parsing

} // namespace tex

#endif // LIBTYPESET_PARSING_MODE_H

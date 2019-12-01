// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_PARSING_MODE_H
#define LIBTYPESET_PARSING_MODE_H

#include "tex/token.h"

#include <vector>

namespace tex
{

namespace parsing
{

class TypesettingMachine;

class LIBTYPESET_API Mode
{
public:
  Mode(TypesettingMachine& m);
  virtual ~Mode() = default;

  TypesettingMachine& machine() const;
  Mode* parent() const;

  bool done() const;

  virtual bool write(Token&& t) = 0;

protected:
  void setDone();

private:
  TypesettingMachine& m_machine;
  Mode* m_parent;
  bool m_done = false;
};

} // namespace parsing

} // namespace tex

namespace tex
{

namespace parsing
{

inline TypesettingMachine& Mode::machine() const
{
  return m_machine;
}

inline bool Mode::done() const
{
  return m_done;
}

inline void Mode::setDone()
{
  m_done = true;
}

} // namespace parsing

} // namespace tex

#endif // LIBTYPESET_PARSING_MODE_H

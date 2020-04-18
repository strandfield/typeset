// Copyright (C) 2019-2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef TYPESET_PAGEEDITOR_MODE_H
#define TYPESET_PAGEEDITOR_MODE_H

#include "tex/token.h"

#include <vector>

class TypesettingMachine;

class LIBTYPESET_API Mode
{
public:
  Mode(TypesettingMachine& m);
  virtual ~Mode() = default;

  enum class Kind
  {
    Horizontal,
    Vertical,
    Math,
  };

  TypesettingMachine& machine() const;
  Mode& parent() const;

  virtual Kind kind() const = 0;

  virtual void write(tex::parsing::Token& t) = 0;
  virtual void finish() = 0;

  template<typename T>
  T& as()
  {
    return static_cast<T&>(*this);
  }

private:
  TypesettingMachine& m_machine;
  Mode* m_parent;
};

inline TypesettingMachine& Mode::machine() const
{
  return m_machine;
}

#endif // TYPESET_PAGEEDITOR_MODE_H

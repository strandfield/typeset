// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_MATHCODE_H
#define LIBTYPESET_MATHCODE_H

#include "tex/defs.h"

#include <array>

namespace tex
{

class MathCode
{
private:
  int m_value;

public:
  explicit MathCode(int val);

  int c() const;
  int f() const;
  int a() const;

  typedef std::array<MathCode, 256> Table;

  static const Table DefaultTable;
};

} // namespace tex

namespace tex
{

inline MathCode::MathCode(int val)
  : m_value(val)
{

}

inline int MathCode::c() const
{
  return m_value / 4096;
}

inline int MathCode::f() const
{
  return (m_value % 4096) / 256;
}

inline int MathCode::a() const
{
  return m_value % 256;
}

} // namespace tex

#endif // LIBTYPESET_MATHCODE_H

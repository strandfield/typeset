// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_DIMEN_H
#define LIBTYPESET_DIMEN_H

#include "tex/units.h"

#include <cassert>
#include <limits>

namespace tex
{

inline float PointPerPica()
{
  return 12.f;
}

inline float PointPerInch()
{
  return 72.27f;
}

inline float BigPointPerInch()
{
  return 72.f;
}

inline float CentimeterPerInch()
{
  return 2.54f;
}

class LIBTYPESET_API Dimen
{
private:
  float m_value; 
  Unit m_unit;

public:

  Dimen() = delete;
  Dimen(float v, Unit u);
  Dimen(float v, PhysicalUnit u);

  float value() const;
  Unit unit() const;

  bool isFinite() const;

  float operator()(const UnitSystem& system) const;
};

inline Dimen::Dimen(float v, Unit u)
  : m_value(v),
    m_unit(u)
{
  
}

inline Dimen::Dimen(float v, PhysicalUnit u)
  : m_value(v),
    m_unit(Unit::Pt)
{
  switch (u)
  {
  case PhysicalUnit::Point:
    break;
  case PhysicalUnit::Pica:
    m_value *= PointPerPica();
    break;
  case PhysicalUnit::Inch:
    m_value *= PointPerInch();
    break;
  case PhysicalUnit::BigPoint: // 72 bp = 1in
    m_value *= PointPerInch() / 72.f;
    break;
  case PhysicalUnit::Centimeter:
    m_value *= PointPerInch() / CentimeterPerInch();
    break;
  case PhysicalUnit::Millimeter:
    m_value *= 0.1f * PointPerInch() / CentimeterPerInch();
    break;
  case PhysicalUnit::DidotPoint: // 1157 dd == 1238 pt
    m_value *= (1238.f / 1157.f);
    break;
  case PhysicalUnit::Cicero: // 1 cc = 12 dd
    m_value *= 12.f * (1238.f / 1157.f);
    break;
  }
}

inline float Dimen::value() const
{
  return m_value;
}

inline Unit Dimen::unit() const
{
  return m_unit;
}

inline bool Dimen::isFinite() const
{
  return m_unit != Unit::Fil && m_unit != Unit::Fill && m_unit != Unit::Filll;
}

inline float Dimen::operator()(const UnitSystem& system) const
{
  switch (m_unit)
  {
  case Unit::Pt:
    return system.pt * value();
  case Unit::Em:
    return system.em * value();
  case Unit::Ex:
    return system.ex * value();
  default:
    assert(false);
    return std::numeric_limits<float>::max();
  }
}

inline Dimen operator*(const Dimen& lhs, float rhs)
{
  return Dimen(lhs.value() *rhs, lhs.unit());
}

inline Dimen operator*(float lhs, const Dimen& rhs)
{
  return Dimen(lhs * rhs.value(), rhs.unit());
}

namespace literals
{

inline Dimen operator""_pt(long double val)
{
  return Dimen(static_cast<float>(val), PhysicalUnit::Point);
}

inline Dimen operator""_pc(long double val)
{
  return Dimen(static_cast<float>(val), PhysicalUnit::Pica);
}

inline Dimen operator""_in(long double val)
{
  return Dimen(static_cast<float>(val), PhysicalUnit::Inch);
}

inline Dimen operator""_cm(long double val)
{
  return Dimen(static_cast<float>(val), PhysicalUnit::Centimeter);
}

inline Dimen operator""_mm(long double val)
{
  return Dimen(static_cast<float>(val), PhysicalUnit::Millimeter);
}

inline Dimen operator""_em(long double val)
{
  return Dimen(static_cast<float>(val), Unit::Em);
}

inline Dimen operator""_ex(long double val)
{
  return Dimen(static_cast<float>(val), Unit::Ex);
}

inline Dimen operator""_fil(long double val)
{
  return Dimen(static_cast<float>(val), Unit::Fil);
}

inline Dimen operator""_fill(long double val)
{
  return Dimen(static_cast<float>(val), Unit::Fill);
}

inline Dimen operator""_filll(long double val)
{
  return Dimen(static_cast<float>(val), Unit::Filll);
}

} // namespace literals

} // namespace tex

#endif // LIBTYPESET_DIMEN_H

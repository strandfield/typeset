// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_UNITS_H
#define LIBTYPESET_UNITS_H

#include "tex/defs.h"

#include <stdexcept>

namespace tex
{

enum class PhysicalUnit
{
  Point,
  Pica,
  Inch,
  BigPoint,
  Centimeter,
  Millimeter,
  DidotPoint,
  Cicero
};

enum class Unit
{
  Pt,
  Em,
  Ex,
  Fil,
  Fill,
  Filll,
};

class LIBTYPESET_API UnitSystem
{
public:
  float pt;
  float em;
  float ex;
};

inline bool str2phyunit(const char* str, PhysicalUnit& result)
{
  if (str == nullptr || str[0] == '\0' || str[1] == '\0' || str[2] != '\0')
    return false;

  if (str[0] == 'p' && str[1] == 't')
    return result = PhysicalUnit::Point, true;
  else if(str[0] == 'p' && str[1] == 'c')
    return result = PhysicalUnit::Pica, true;
  else if (str[0] == 'i' && str[1] == 'n')
    return result = PhysicalUnit::Inch, true;
  else if (str[0] == 'b' && str[1] == 'p')
    return result = PhysicalUnit::BigPoint, true;
  else if (str[0] == 'c' && str[1] == 'm')
    return result = PhysicalUnit::Centimeter, true;
  else if (str[0] == 'm' && str[1] == 'm')
    return result = PhysicalUnit::Millimeter, true;
  else if (str[0] == 'd' && str[1] == 'd')
    return result = PhysicalUnit::DidotPoint, true;
  else if (str[0] == 'c' && str[1] == 'c')
    return result = PhysicalUnit::Cicero, true;

  return false;
}

inline bool str2unit(const char* str, Unit& result)
{
  const char* reprs[] = {
    "pt",
    "em",
    "ex",
    "fil",
    "fill",
    "filll"
  };

  const Unit units[] = {
    Unit::Pt,
    Unit::Em,
    Unit::Ex,
    Unit::Fil,
    Unit::Fill,
    Unit::Filll,
  };

  auto comp = [](const char* a, const char* b) -> bool
  {
    while (*a == *b && *a != '\0' && *b != '\0')
      ++a, ++b;

    return *a == *b;
  };

  for (size_t i(0); i < 6; ++i)
  {
    if (comp(str, reprs[i]))
      return result = units[i], true;
  }

  return false;
}

} // namespace tex

#endif // LIBTYPESET_UNITS_H

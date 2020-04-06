// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_CHARACTERBOX_H
#define LIBTYPESET_CHARACTERBOX_H

#include "tex/box.h"
#include "tex/unicode.h"

namespace tex
{

// @TODO: should this class also store the Font & FontSize used ?
class LIBTYPESET_API CharacterBox : public Box
{
private:
  Character m_char;

public:
  CharacterBox(Character c, const BoxMetrics& metrics)
    : Box(metrics),
      m_char(c)
  {
    
  }

  Character character() const { return m_char; }
};

} // namespace tex

#endif // LIBTYPESET_CHARACTERBOX_H

// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_CHARACTERBOX_H
#define LIBTYPESET_CHARACTERBOX_H

#include "tex/box.h"
#include "tex/font.h"
#include "tex/unicode.h"

namespace tex
{

class LIBTYPESET_API CharacterBox : public Box
{
private:
  Character m_char;
  Font m_font;

public:
  CharacterBox(Character c, Font f, const BoxMetrics& metrics)
    : Box(metrics),
      m_char(c),
      m_font(f)
  {
    
  }

  Character character() const { return m_char; }
  Font font() const { return m_font; }

  bool isCharacterBox() const override { return true; }
};

} // namespace tex

#endif // LIBTYPESET_CHARACTERBOX_H

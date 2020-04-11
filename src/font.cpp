// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/font.h"

#include <algorithm>

namespace tex
{

const Font Font::Default = Font{ 0 };
const Font Font::Text = Font{ 1 };
const Font Font::MathRoman = Font{ 2 };
const Font Font::MathItalic = Font{ 3 };

Font::Font()
  : mFont(0)
{

}

} // namespace tex

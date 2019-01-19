// Copyright (C) 2018 Vincent Chambrin
// This file is part of the liblayout project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/font.h"

#include <algorithm>

namespace tex
{

const Font Font::Default = Font{ 0 };
const Font Font::Text = Font{ 1 };
const Font Font::MathRoman = Font{ 2 };
const Font Font::MathItalic = Font{ 3 };

const FontSize FontSize::Normal = FontSize{ 6 };
const FontSize FontSize::Small = FontSize{ 5 };
const FontSize FontSize::Large = FontSize{ 7 };
const FontSize FontSize::FootNoteSize = FontSize{ 4 };
const FontSize FontSize::ScriptSize = FontSize{ 3 };
const FontSize FontSize::Tiny = FontSize{ 1 };
const FontSize FontSize::LARGE = FontSize{ 9 };
const FontSize FontSize::Huge = FontSize{ 10 };
const FontSize FontSize::HUGE = FontSize{ 11 };

FontSize FontSize::larger() const
{
  return FontSize{ std::min(mSize + 1, 11) };
}

FontSize FontSize::smaller() const
{
  return FontSize{ std::max(mSize - 1, 1) };
}

} // namespace tex

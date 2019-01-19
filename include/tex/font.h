// Copyright (C) 2018 Vincent Chambrin
// This file is part of the liblayout project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBLAYOUT_FONT_H
#define LIBLAYOUT_FONT_H

#include "tex/defs.h"

namespace tex
{

class LIBLAYOUT_API Font
{
public:
  explicit Font(int val) : mFont(val) { }
  Font(const Font & ) = default;
  ~Font() = default;

  static const Font Default;
  static const Font Text;
  static const Font MathRoman;
  static const Font MathItalic;

  inline int id() const { return mFont; }

  Font & operator=(const Font & ) = default;

private:
  int mFont;
};

inline bool operator==(const Font & a, const Font & b) { return a.id() == b.id(); }
inline bool operator!=(const Font & a, const Font & b) { return b.id() != a.id(); }


class LIBLAYOUT_API FontSize
{
public:
  explicit FontSize(int val) : mSize(val) { }
  FontSize(const FontSize & ) = default;
  ~FontSize() = default;

  inline bool isTextSize() const { return mSize >= 5; }
  inline bool isScriptSize() const { return mSize == 3 || mSize == 4; }
  inline bool isScriptScriptSize() const { return mSize == 1 || mSize == 2; }

  inline int value() const { return mSize; }

  FontSize larger() const;
  FontSize smaller() const;

  static const FontSize Normal;
  static const FontSize Small;
  static const FontSize Large;
  static const FontSize FootNoteSize;
  static const FontSize ScriptSize;
  static const FontSize Tiny;
  static const FontSize LARGE;
  static const FontSize Huge;
  static const FontSize HUGE;

  FontSize & operator=(const FontSize &) = default;

private:
  int mSize;
};

inline bool operator==(const FontSize & a, const FontSize & b) { return a.value() == b.value(); }
inline bool operator!=(const FontSize & a, const FontSize & b) { return a.value() != b.value(); }

} // namespace tex

#endif // LIBLAYOUT_FONT_H

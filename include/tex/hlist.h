// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_HLIST_H
#define LIBTYPESET_HLIST_H

#include "tex/font.h"
#include "tex/listbox.h"
#include "tex/unicode.h"

namespace tex
{

class Kern;
class TypesetEngine;

class LIBTYPESET_API HListBuilder
{
public:
  List result;
  std::shared_ptr<TypesetEngine> typeset;
  tex::Font font;
  int spacefactor = 1000;

  explicit HListBuilder(std::shared_ptr<TypesetEngine> e, tex::Font f = tex::Font(0));

  void push_back(tex::Character c);
  void push_back_interword_glue();
  void push_back(std::shared_ptr<tex::Box> b);
  void push_back(std::shared_ptr<tex::Glue> g);
  void push_back(std::shared_ptr<tex::Kern> k);
};

} // namespace tex

#endif // LIBTYPESET_HLIST_H

// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/hlist.h"

#include "tex/glue.h"
#include "tex/kern.h"
#include "tex/typeset.h"

#include <algorithm>
#include <cassert>

namespace tex
{

HListBuilder::HListBuilder(std::shared_ptr<TypesetEngine> e, tex::Font f)
  : typeset(e),
    font(f)
{

}

void HListBuilder::push_back(tex::Character c)
{
  auto box = typeset->typeset(c, font);
  result.push_back(box);

  int g = typeset->metrics()->sfcode(c);

  if (g != 0)
  {
    if (spacefactor < 1000 && g > 1000)
      spacefactor = 1000;
    spacefactor = g;
  }
}

void HListBuilder::push_back_interword_glue()
{
  float space = typeset->metrics()->interwordSpace(font);
  float stretch = typeset->metrics()->interwordStretch(font);
  float shrink = typeset->metrics()->interwordShrink(font);

  if (spacefactor >= 2000)
    space += typeset->metrics()->extraSpace(font);

  stretch *= (spacefactor / 1000.f);
  shrink *= (1000.f / spacefactor);

  push_back(tex::glue(space, Stretch(stretch), Shrink(shrink)));
}

void HListBuilder::push_back(std::shared_ptr<tex::Box> b)
{
  result.push_back(b);
  spacefactor = 1000;
}

void HListBuilder::push_back(std::shared_ptr<tex::Glue> g)
{
  result.push_back(g);
}

void HListBuilder::push_back(std::shared_ptr<tex::Kern> k)
{
  result.push_back(k);
}

} // namespace tex

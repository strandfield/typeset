// Copyright (C) 2018 Vincent Chambrin
// This file is part of the liblayout project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/hbox.h"

#include "tex/kern.h"

#include <algorithm>
#include <cassert>

namespace tex
{

HBox::HBox(List && list, float w, float h, float d, float shift)
  : ListBox(std::move(list), w, h, d, shift)
{

}

std::shared_ptr<HBox> hbox(List && hlist)
{
  float height = 0;
  float depth = 0;
  float width = 0;

  for (auto it = hlist.begin(); it != hlist.end(); ++it)
  {
    if ((*it)->isBox())
    {
      auto box = std::static_pointer_cast<Box>(*it);

      if ((*it)->isDerivedFrom<ListBox>())
      {
        auto listbox = std::static_pointer_cast<ListBox>(box);

        height = std::max(height, box->height() - listbox->shiftAmount());
        depth = std::max(depth, box->depth() + listbox->shiftAmount());
      }
      else
      {
        height = std::max(height, box->height());
        depth = std::max(depth, box->depth());
      }
      width += box->width();
    }
    else if ((*it)->isKern())
      width += std::static_pointer_cast<Kern>(*it)->space();
    else if ((*it)->isGlue())
      width += std::static_pointer_cast<Glue>(*it)->space();
  }

  return std::make_shared<HBox>(std::move(hlist), width, height, depth);
}

std::shared_ptr<HBox> hbox(std::initializer_list<std::shared_ptr<Node>> && nodes)
{
  return hbox(List{ nodes });
}

std::shared_ptr<HBox> hbox(List && hlist, float w)
{
  float height = 0;
  float depth = 0;
  float naturalWidth = 0;
  GlueStretch stretch;
  GlueShrink shrink;

  for (auto it = hlist.begin(); it != hlist.end(); ++it)
  {
    if ((*it)->isBox())
    {
      auto box = std::static_pointer_cast<Box>(*it);

      if ((*it)->isDerivedFrom<ListBox>())
      {
        auto listbox = std::static_pointer_cast<ListBox>(box);

        height = std::max(height, listbox->height() - listbox->shiftAmount());
        depth = std::max(depth, listbox->depth() + listbox->shiftAmount());
      }
      else
      {
        height = std::max(height, box->height());
        depth = std::max(depth, box->depth());
      }
      naturalWidth += box->width();
    }
    else if ((*it)->isKern())
    {
      naturalWidth += std::static_pointer_cast<Kern>(*it)->space();
    }
    else if ((*it)->isGlue())
    {
      auto glue = std::static_pointer_cast<Glue>(*it);
      naturalWidth += glue->space();
      glue->accumulate(shrink, stretch);
    }
  }

  auto box = std::make_shared<HBox>(std::move(hlist), naturalWidth, height, depth);

  const float final_width = box->setGlue(naturalWidth, w, shrink, stretch);
  box->setWidth(final_width);
  return box;
}

void raise(std::shared_ptr<HBox> box, float amount)
{
  box->setShiftAmount(-amount);
}

void lower(std::shared_ptr<HBox> box, float amount)
{
  box->setShiftAmount(amount);
}

} // namespace tex

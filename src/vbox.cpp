// Copyright (C) 2018 Vincent Chambrin
// This file is part of the liblayout project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/vbox.h"

#include "tex/kern.h"

#include <algorithm>

namespace tex
{

VBox::VBox(List && list, float w, float h, float d)
  : ListBox(std::move(list), w, h, d)
{

}

std::shared_ptr<VBox> vbox(List && list)
{
  float height = 0;
  float width = 0;

  auto last_box_it = list.end();
  for (auto it = list.begin(); it != list.end(); ++it)
  {
    if ((*it)->isBox())
    {
      last_box_it = it;
      height += std::static_pointer_cast<Box>(*it)->totalHeight();
      width = std::max(std::static_pointer_cast<Box>(*it)->width(), width);
    }
    else if ((*it)->isKern())
    {
      height += std::static_pointer_cast<Kern>(*it)->space();
    }
    else if ((*it)->isGlue()) 
    {
      height += std::static_pointer_cast<Glue>(*it)->space();
    }
  }

  float depth;
  if (last_box_it == list.end())
  {
    depth = 0.f;
  }
  else
  {
    auto it = last_box_it;
    if (++it != list.end()) // the box is followed by kern or glue
    {
      depth = 0.f;
    }
    else
    {
      depth = std::static_pointer_cast<Box>(*last_box_it)->depth();
      height -= depth;
    }
  }

  return std::make_shared<VBox>(std::move(list), width, height, depth);
}


std::shared_ptr<VBox> vtop(List && list)
{
  auto box = vbox(std::move(list));

  float h = box->height();
  float d = box->depth();

  float x = (*box->list().begin())->isBox() ? std::static_pointer_cast<Box>(*box->list().begin())->height() : 0.f;
  box->setHeight(x);
  box->setDepth(h + d - x);

  return box;
}

} // namespace tex

// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/hbox.h"

#include "tex/kern.h"

#include <algorithm>
#include <cassert>

namespace tex
{

HBox::HBox(List && list)
  : ListBox(std::move(list))
{
  rebox();
}

HBox::HBox(List && list, float desiredWidth)
  : ListBox(std::move(list))
{
  rebox(desiredWidth);
}


void HBox::getBoxingInfo(float *width, float *height, float *depth, GlueShrink *shrink, GlueStretch *stretch) const
{
  float h = 0;
  float d = 0;
  float w = 0;

  for (auto it = list().begin(); it != list().end(); ++it)
  {
    if ((*it)->isBox())
    {
      auto box = std::static_pointer_cast<Box>(*it);

      if ((*it)->isDerivedFrom<ListBox>())
      {
        auto listbox = std::static_pointer_cast<ListBox>(box);

        h = std::max(h, listbox->height() - listbox->shiftAmount());
        d = std::max(d, listbox->depth() + listbox->shiftAmount());
      }
      else
      {
        h = std::max(h, box->height());
        d = std::max(d, box->depth());
      }
      w += box->width();
    }
    else if ((*it)->isKern())
    {
      w += std::static_pointer_cast<Kern>(*it)->space();
    }
    else if ((*it)->isGlue())
    {
      auto glue = std::static_pointer_cast<Glue>(*it);
      w += glue->space();
      glue->accumulate(*shrink, *stretch);
    }
  }

  if (height)
    *height = h;
  if (depth)
    *depth = d;
  if (width)
    *width = w;
}

void HBox::rebox()
{
  float natural_width, height, depth;
  GlueShrink shrink;
  GlueStretch stretch;

  getBoxingInfo(&natural_width, &height, &depth, &shrink, &stretch);

  setHeight(height);
  setDepth(depth);
  setWidth(natural_width);
}

BoxingResult HBox::rebox(float desiredWidth)
{
  float natural_width, height, depth;
  GlueShrink shrink;
  GlueStretch stretch;

  getBoxingInfo(&natural_width, &height, &depth, &shrink, &stretch);

  setHeight(height);
  setDepth(depth);
 
  float final_width = setGlue(natural_width, desiredWidth, shrink, stretch);
  setWidth(final_width);

  if (final_width < desiredWidth)
    return BoxingResult::UnderfullBox;
  else if (final_width > desiredWidth)
    return BoxingResult::OverfullBox;
  return BoxingResult::NormalBox;
}

std::shared_ptr<HBox> hbox(List && hlist)
{
  return std::make_shared<HBox>(std::move(hlist));
}

std::shared_ptr<HBox> hbox(std::initializer_list<std::shared_ptr<Node>> && nodes)
{
  return hbox(List{ nodes });
}

std::shared_ptr<HBox> hbox(List && hlist, float w)
{
  return std::make_shared<HBox>(std::move(hlist), w);
}

void raise(std::shared_ptr<HBox> box, float amount)
{
  box->setShiftAmount(-amount);
}

void lower(std::shared_ptr<HBox> box, float amount)
{
  box->setShiftAmount(amount);
}

HBoxEditor::HBoxEditor(HBox & box)
  : mReboxDone(false)
  , mHbox(&box)
{

}

HBoxEditor::~HBoxEditor()
{
  if (!mReboxDone)
    mHbox->rebox();
}

List & HBoxEditor::list()
{
  return mHbox->mutableList();
}

void HBoxEditor::rebox()
{
  mReboxDone = true;
  mHbox->rebox();
}

BoxingResult HBoxEditor::rebox(float desiredWidth)
{
  mReboxDone = true;
  return mHbox->rebox(desiredWidth);
}

} // namespace tex

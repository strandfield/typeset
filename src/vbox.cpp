// Copyright (C) 2018 Vincent Chambrin
// This file is part of the liblayout project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/vbox.h"

#include "tex/kern.h"

#include <algorithm>

namespace tex
{

VBox::VBox(List && list)
  : ListBox(std::move(list))
{
  rebox_vbox();
}

VBox::VBox(List && list, float desiredHeight)
  : ListBox(std::move(list))
{
  rebox_vbox(desiredHeight);
}

void VBox::getBoxingInfo(float *width, float *height, float *depth, GlueShrink *shrink, GlueStretch *stretch) const
{
  float h = 0;
  float w = 0;

  auto last_box_it = list().end();
  for (auto it = list().begin(); it != list().end(); ++it)
  {
    if ((*it)->isBox())
    {
      last_box_it = it;
      h += std::static_pointer_cast<Box>(*it)->totalHeight();
      w = std::max(std::static_pointer_cast<Box>(*it)->width(), w);
    }
    else if ((*it)->isKern())
    {
      h += std::static_pointer_cast<Kern>(*it)->space();
    }
    else if ((*it)->isGlue())
    {
      auto glue = std::static_pointer_cast<Glue>(*it);
      h += std::static_pointer_cast<Glue>(*it)->space();
      glue->accumulate(*shrink, *stretch);
    }
  }

  float d;
  if (last_box_it == list().end())
  {
    d = 0.f;
  }
  else
  {
    auto it = last_box_it;
    if (++it != list().end()) // the box is followed by kern or glue
    {
      d = 0.f;
    }
    else
    {
      d = std::static_pointer_cast<Box>(*last_box_it)->depth();
      h -= d;
    }
  }

  if (height)
    *height = h;
  if (depth)
    *depth = d;
  if (width)
    *width = w;
}

void VBox::rebox_vbox()
{
  float width, height, depth;
  GlueShrink shrink;
  GlueStretch stretch;

  getBoxingInfo(&width, &height, &depth, &shrink, &stretch);

  setHeight(height);
  setDepth(depth);
  setWidth(width);
}

BoxingResult VBox::rebox_vbox(float desiredHeight)
{
  float width, height, depth;
  GlueShrink shrink;
  GlueStretch stretch;

  getBoxingInfo(&width, &height, &depth, &shrink, &stretch);

  setWidth(width);
  setDepth(depth);

  float final_height = setGlue(height, desiredHeight, shrink, stretch);
  setHeight(final_height);

  if (final_height < desiredHeight)
    return BoxingResult::UnderfullBox;
  else if (final_height > desiredHeight)
    return BoxingResult::OverfullBox;
  return BoxingResult::NormalBox;
}

void VBox::rebox_vtop()
{
  rebox_vbox();
  make_vtop();
}

BoxingResult VBox::rebox_vtop(float desiredHeight)
{
  BoxingResult ret = rebox_vbox(desiredHeight);
  make_vtop();
  return ret;
}

void VBox::make_vtop()
{
  float h = height();
  float d = depth();

  float x = (*list().begin())->isBox() ? std::static_pointer_cast<Box>(*list().begin())->height() : 0.f;
  setHeight(x);
  setDepth(h + d - x);
}


std::shared_ptr<VBox> vbox(List && list)
{
  return std::make_shared<VBox>(std::move(list));
}

std::shared_ptr<VBox> vbox(List && list, float h)
{
  return std::make_shared<VBox>(std::move(list), h);
}

std::shared_ptr<VBox> vtop(List && list)
{
  auto box = vbox(std::move(list));
  box->make_vtop();
  return box;
}

std::shared_ptr<VBox> vtop(List && list, float dimh)
{
  auto box = vbox(std::move(list), dimh);
  box->make_vtop();
  return box;
}

VBoxEditor::VBoxEditor(VBox & box)
  : mReboxDone(false)
  , mVbox(&box)
{

}

VBoxEditor::~VBoxEditor()
{
  if (!mReboxDone)
    mVbox->rebox_vbox();
}

List & VBoxEditor::list()
{
  return mVbox->mutableList();
}

void VBoxEditor::rebox()
{
  mReboxDone = true;
  mVbox->rebox_vbox();
}

BoxingResult VBoxEditor::rebox(float desiredHeight)
{
  mReboxDone = true;
  return mVbox->rebox_vbox(desiredHeight);
}

void VBoxEditor::rebox_vtop()
{
  mReboxDone = true;
  mVbox->rebox_vtop();
}

BoxingResult VBoxEditor::rebox_vtop(float desiredHeight)
{
  mReboxDone = true;
  return mVbox->rebox_vtop(desiredHeight);
}

} // namespace tex

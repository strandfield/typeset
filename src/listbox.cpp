// Copyright (C) 2018 Vincent Chambrin
// This file is part of the liblayout project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/listbox.h"

#include "tex/kern.h"

namespace tex
{

ListBox::ListBox(List && list, float w, float h, float d, float shift)
  : mList(std::move(list))
  , mWidth(w)
  , mHeight(h)
  , mDepth(d)
  , mShiftAmount(shift)
  , mGlueSettings{0.f, GlueOrder::Normal}
{

}

void ListBox::setGlue(float ratio, GlueOrder order)
{
  mGlueSettings.ratio = ratio;
  mGlueSettings.order = order;
}

float ListBox::setGlue(float x, float desired, const GlueShrink & shrink, const GlueStretch & stretch)
{
  if (x < desired)
  {
    if (stretch.filll != 0.f)
    {
      this->setGlue((desired - x) / stretch.filll, GlueOrder::Filll);
      return desired;
    }
    else if (stretch.fill != 0.f)
    {
      this->setGlue((desired - x) / stretch.fill, GlueOrder::Fill);
      return desired;
    }
    else if (stretch.fil != 0.f)
    {
      this->setGlue((desired - x) / stretch.fil, GlueOrder::Fil);
      return desired;
    }
    else if (stretch.normal != 0.f)
    {
      this->setGlue((desired - x) / stretch.normal, GlueOrder::Normal);
      return desired;
    }
    else
    {
      this->setGlue(0.f, GlueOrder::Normal);
      return x;
    }
  }
  else if (x > desired)
  {
    if (shrink.filll != 0.f)
    {
      this->setGlue((desired - x) / shrink.filll, GlueOrder::Filll);
      return desired;
    }
    else if (shrink.fill != 0.f)
    {
      this->setGlue((desired - x) / shrink.fill, GlueOrder::Fill);
      return desired;
    }
    else if (shrink.fil != 0.f)
    {
      this->setGlue((desired - x) / shrink.fil, GlueOrder::Fil);
      return desired;
    }
    else if (shrink.normal != 0.f)
    {
      float r = (desired - x) / shrink.normal;
      if (r < -1.f)
        r = -1.f;
      this->setGlue(r, GlueOrder::Normal);
      return x + r * shrink.normal;
    }
    else
    {
      this->setGlue(0.f, GlueOrder::Normal);
      return x;
    }
  }

  return x;
}

float naturalWidth(const List & l)
{
  float result = 0;

  for (auto it = l.begin(); it != l.end(); ++it)
  {
    if ((*it)->isBox())
    {
      auto box = std::static_pointer_cast<Box>(*it);
      result += box->width();
    }
    else if ((*it)->isKern())
    {
      result += std::static_pointer_cast<Kern>(*it)->space();
    }
    else if ((*it)->isGlue())
    {
      auto glue = std::static_pointer_cast<Glue>(*it);
      result += glue->space();
    }
  }

  return result;
}

float naturalWidth(const List & l, GlueShrink & shrink, GlueStretch & stretch)
{
  float result = 0;

  for (auto it = l.begin(); it != l.end(); ++it)
  {
    if ((*it)->isBox())
    {
      auto box = std::static_pointer_cast<Box>(*it);
      result += box->width();
    }
    else if ((*it)->isKern())
    {
      result += std::static_pointer_cast<Kern>(*it)->space();
    }
    else if ((*it)->isGlue())
    {
      auto glue = std::static_pointer_cast<Glue>(*it);
      result += glue->space();
      glue->accumulate(shrink, stretch);
    }
  }

  return result;
}

} // namespace tex

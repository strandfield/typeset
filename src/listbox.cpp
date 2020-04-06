// Copyright (C) 2019-2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/listbox.h"

#include "tex/kern.h"

namespace tex
{

ListBox::ListBox(List && list)
  : mList(std::move(list))
  , mShiftAmount(0)
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

ListBoxEditor::ListBoxEditor(ListBox & box)
  : mListBox(&box)
{

}

ListBoxEditor::~ListBoxEditor()
{

}

List & ListBoxEditor::list()
{
  return mListBox->mutableList();
}

void ListBoxEditor::enlarge(float amount)
{
  mListBox->setWidth(mListBox->width() + amount);
}

void ListBoxEditor::setHeight(float h)
{
  mListBox->setHeight(h);
}

void ListBoxEditor::increaseDepth(float amount)
{
  mListBox->setDepth(mListBox->depth() + amount);
}

void ListBoxEditor::setDepth(float d)
{
  mListBox->setDepth(d);
}

} // namespace tex

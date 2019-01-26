// Copyright (C) 2018 Vincent Chambrin
// This file is part of the liblayout project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBLAYOUT_LISTBOX_H
#define LIBLAYOUT_LISTBOX_H

#include "tex/box.h"

#include "tex/glue.h"

#include <list>
#include <memory>

namespace tex
{

typedef std::list<std::shared_ptr<Node>> List;

enum BoxingResult {
  NormalBox,
  OverfullBox,
  UnderfullBox,
};

class LIBLAYOUT_API ListBox : public Box
{
public:
  ~ListBox() = default;

  inline float height() const override { return mHeight; }
  inline float depth() const override { return mDepth; }
  inline float width() const override { return mWidth; }

  inline float shiftAmount() const { return mShiftAmount; }
  inline void setShiftAmount(float sa) { mShiftAmount = sa; }
  inline void shift(float amount) { mShiftAmount += amount; }

  inline const List & list() const { return mList; }

  inline float glueRatio() const { return mGlueSettings.ratio; }
  inline GlueOrder glueOrder() const { return mGlueSettings.order; }

protected:
  friend class ListBoxEditor;

  ListBox(List && list);

  inline List & mutableList() { return mList; }
  inline void setHeight(float h) { mHeight = h; }
  inline void setDepth(float d) { mDepth = d; }
  inline void setWidth(float w) { mWidth = w; }

  void setGlue(float ratio, GlueOrder order);
  float setGlue(float x, float desired, const GlueShrink & shrink, const GlueStretch & stretch);

private:
  List mList;
  float mWidth;
  float mHeight;
  float mDepth;
  float mShiftAmount;
  GlueSettings mGlueSettings;
};

class LIBLAYOUT_API ListBoxEditor
{
private:
  ListBox* mListBox;
public:
  ListBoxEditor(ListBox & box);
  ~ListBoxEditor();

  List & list();

  void enlarge(float amount);
  void increaseDepth(float amount);
  void setHeight(float h);
  void setDepth(float d);
};

} // namespace tex

#endif // LIBLAYOUT_LISTBOX_H

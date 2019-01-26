// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_HBOX_H
#define LIBTYPESET_HBOX_H

#include "tex/listbox.h"

namespace tex
{

class LIBTYPESET_API HBox final : public ListBox
{
public:
  HBox(List && list);
  HBox(List && list, float desiredWidth);
  ~HBox() = default;

  void getBoxingInfo(float *width, float *height, float *depth, GlueShrink *shrink, GlueStretch *stretch) const;

protected:
  friend class HBoxEditor;

  void rebox();
  BoxingResult rebox(float desiredWidth);
};

LIBTYPESET_API std::shared_ptr<HBox> hbox(List && hlist);
LIBTYPESET_API std::shared_ptr<HBox> hbox(std::initializer_list<std::shared_ptr<Node>> && nodes);
LIBTYPESET_API std::shared_ptr<HBox> hbox(List && hlist, float w);

LIBTYPESET_API void raise(std::shared_ptr<HBox> box, float amount);
LIBTYPESET_API void lower(std::shared_ptr<HBox> box, float amount);

class LIBTYPESET_API HBoxEditor final
{
private:
  bool mReboxDone;
  HBox* mHbox;
public:
  HBoxEditor(HBox & box);
  ~HBoxEditor();

  List & list();

  void rebox();
  BoxingResult rebox(float desiredWidth);
};

} // namespace tex

#endif // LIBTYPESET_HBOX_H

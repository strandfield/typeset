// Copyright (C) 2018 Vincent Chambrin
// This file is part of the liblayout project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBLAYOUT_HBOX_H
#define LIBLAYOUT_HBOX_H

#include "tex/listbox.h"

namespace tex
{

class LIBLAYOUT_API HBox final : public ListBox
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

LIBLAYOUT_API std::shared_ptr<HBox> hbox(List && hlist);
LIBLAYOUT_API std::shared_ptr<HBox> hbox(std::initializer_list<std::shared_ptr<Node>> && nodes);
LIBLAYOUT_API std::shared_ptr<HBox> hbox(List && hlist, float w);

LIBLAYOUT_API void raise(std::shared_ptr<HBox> box, float amount);
LIBLAYOUT_API void lower(std::shared_ptr<HBox> box, float amount);

class LIBLAYOUT_API HBoxEditor final
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

#endif // LIBLAYOUT_HBOX_H

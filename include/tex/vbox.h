// Copyright (C) 2018 Vincent Chambrin
// This file is part of the liblayout project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBLAYOUT_VBOX_H
#define LIBLAYOUT_VBOX_H

#include "tex/listbox.h"

namespace tex
{

class LIBLAYOUT_API VBox final : public ListBox
{
public:
  explicit VBox(List && list);
  VBox(List && list, float desiredHeight);
  ~VBox() = default;

  void getBoxingInfo(float *width, float *height, float *depth, GlueShrink *shrink, GlueStretch *stretch) const;

protected:
  friend class VBoxEditor;
  friend LIBLAYOUT_API std::shared_ptr<VBox> vtop(List && list);
  friend LIBLAYOUT_API std::shared_ptr<VBox> vtop(List && list, float h);

  void rebox_vbox();
  BoxingResult rebox_vbox(float desiredHeight);
  void rebox_vtop();
  BoxingResult rebox_vtop(float desiredHeight);
  void make_vtop();
};

LIBLAYOUT_API std::shared_ptr<VBox> vbox(List && list);
LIBLAYOUT_API std::shared_ptr<VBox> vbox(List && list, float h);
LIBLAYOUT_API std::shared_ptr<VBox> vtop(List && list);
LIBLAYOUT_API std::shared_ptr<VBox> vtop(List && list, float h);

class LIBLAYOUT_API VBoxEditor final
{
private:
  bool mReboxDone;
  VBox* mVbox;
public:
  VBoxEditor(VBox & box);
  ~VBoxEditor();

  List & list();

  void rebox();
  BoxingResult rebox(float desiredHeight);
  void rebox_vtop();
  BoxingResult rebox_vtop(float desiredHeight);
};

} // namespace tex

#endif // LIBLAYOUT_VBOX_H

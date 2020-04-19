// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_VBOX_H
#define LIBTYPESET_VBOX_H

#include "tex/listbox.h"

namespace tex
{

class LIBTYPESET_API VListBuilder
{
public:
  List result;
  std::shared_ptr<Glue> baselineskip;
  std::shared_ptr<Glue> lineskip;
  float lineskiplimit = 0.f;
  float prevdepth = -10000.f;

  VListBuilder(std::shared_ptr<Glue> baselineskip_, std::shared_ptr<Glue> lineskip_);
  
  void push_back(const std::shared_ptr<Box>& box);

  static void push_back(List& vlist, const std::shared_ptr<Box>& box, float& prevdepth, const std::shared_ptr<Glue>& baselineskip, const std::shared_ptr<Glue>& lineskip, float lineskiplimit = 0.f);

  void push_back_node(const std::shared_ptr<Node>& node);
};

class LIBTYPESET_API VBox final : public ListBox
{
public:
  explicit VBox(List && list);
  VBox(List && list, float desiredHeight);
  explicit VBox(const BoxMetrics& metrics);
  ~VBox() = default;

  void getBoxingInfo(float *width, float *height, float *depth, GlueShrink *shrink, GlueStretch *stretch) const;

  bool isVBox() const override;

protected:
  friend class VBoxEditor;
  friend LIBTYPESET_API std::shared_ptr<VBox> vtop(List && list);
  friend LIBTYPESET_API std::shared_ptr<VBox> vtop(List && list, float h);

  void rebox_vbox();
  BoxingResult rebox_vbox(float desiredHeight);
  void rebox_vtop();
  BoxingResult rebox_vtop(float desiredHeight);
  void make_vtop();
};

LIBTYPESET_API std::shared_ptr<VBox> vbox(List && list);
LIBTYPESET_API std::shared_ptr<VBox> vbox(List && list, float h);
LIBTYPESET_API std::shared_ptr<VBox> vtop(List && list);
LIBTYPESET_API std::shared_ptr<VBox> vtop(List && list, float h);

class LIBTYPESET_API VBoxEditor final
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

  void changeHeight(float height);
  void done();
};

} // namespace tex

#endif // LIBTYPESET_VBOX_H

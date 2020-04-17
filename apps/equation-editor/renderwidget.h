// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_EQEDITOR_RENDERWIDGET_H
#define LIBTYPESET_EQEDITOR_RENDERWIDGET_H

#include "common/renderwidget.h"

class EquationEditorRenderWidget : public RenderWidget
{
  Q_OBJECT
public:

  void setDrawChars(bool on = true);
  void setDrawCharBoxes(bool on);
  void setDrawListBox(bool on);
  void setDrawBaselines(bool on);

protected:
  void paint(QPainter& painter, const std::shared_ptr<tex::Box>& box, const QPointF& pos) override;

private:
  std::shared_ptr<tex::Box> m_box;
  bool m_draw_chars = true;
  bool m_draw_char_bbox = false;
  bool m_draw_listbox = false;
  bool m_draw_baselines = false;
};

#endif // LIBTYPESET_EQEDITOR_RENDERWIDGET_H

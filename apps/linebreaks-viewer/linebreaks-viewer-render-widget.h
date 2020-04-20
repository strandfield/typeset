// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_LINEBREAKSVIEWER_RENDERWIDGER_H
#define LIBTYPESET_LINEBREAKSVIEWER_RENDERWIDGER_H

#include "common/renderwidget.h"

class LinebreaksViewerRenderWidget : public RenderWidget
{
  Q_OBJECT
public:
  bool show_ratios = false;
public:
  using RenderWidget::RenderWidget;

  void paint(QPainter& painter, const std::shared_ptr<tex::Box>& box, const QPointF& pos) override;


};

#endif // LIBTYPESET_LINEBREAKSVIEWER_RENDERWIDGER_H

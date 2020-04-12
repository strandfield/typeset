// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_EQEDITOR_RENDERWIDGET_H
#define LIBTYPESET_EQEDITOR_RENDERWIDGET_H

#include <QWidget>

#include "tex/box.h"

class RenderWidget : public QWidget
{
  Q_OBJECT
public:

  void setBox(std::shared_ptr<tex::Box> box);

protected:
  void paintEvent(QPaintEvent* ev) override;

private:
  std::shared_ptr<tex::Box> m_box;
};

#endif // LIBTYPESET_EQEDITOR_RENDERWIDGET_H

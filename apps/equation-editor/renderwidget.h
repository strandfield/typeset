// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_EQEDITOR_RENDERWIDGET_H
#define LIBTYPESET_EQEDITOR_RENDERWIDGET_H

#include <QWidget>

class RenderWidget : public QWidget
{
  Q_OBJECT
public:

protected:
  void paintEvent(QPaintEvent* ev) override;
};

#endif // LIBTYPESET_EQEDITOR_RENDERWIDGET_H

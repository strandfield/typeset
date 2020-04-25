// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_APPCOMMON_PAGEWIDGET_H
#define LIBTYPESET_APPCOMMON_PAGEWIDGET_H

#include "renderwidget.h"

namespace tex
{
class Rule;
} // namespace tex

class QScrollBar;

class PageWidget : public QWidget
{
  Q_OBJECT
public:
  explicit PageWidget(QWidget* parent = nullptr);

  void setPageWidth(int w);
  int pagewidth() const;

  int hsize() const;

  void setMargins(QMargins margins);
  const QMargins& margins() const;

  void setBox(std::shared_ptr<tex::Box> box);

protected:
  void paintEvent(QPaintEvent* ev) override;
  void resizeEvent(QResizeEvent* ev) override;

protected:
  void updateLayout();

private:
  int m_pagewidth = 800;
  RenderWidget* m_renderwidget;
  QScrollBar* m_vertical_scrollbar;
};

#endif // LIBTYPESET_APPCOMMON_PAGEWIDGET_H

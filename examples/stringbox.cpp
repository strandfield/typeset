// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "stringbox.h"

#include <QFontMetricsF>

StringBox::StringBox(const QString & t, const QFont & f)
  : text(t)
  , font(f)
{
  QFontMetricsF metrics{ f };
  const QRectF rect = metrics.tightBoundingRect(text);
  width_ = rect.width();
  height_ = -rect.top();
  depth_ = rect.bottom();
}

StringBox::StringBox(const QString & t, const QFont & f, tex::BoxMetrics metrics)
  : text(t)
  , font(f)
{
  width_ = metrics.width;
  height_ = metrics.height;
  depth_ = metrics.depth;
}

float StringBox::height() const
{
  return height_;
}

float StringBox::depth() const
{
  return depth_;
}

float StringBox::width() const
{
  return width_;
}

std::shared_ptr<StringBox> stringbox(const QString & text, const QFont & f)
{
  return std::make_shared<StringBox>(text, f);
}

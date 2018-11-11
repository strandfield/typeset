// Copyright (C) 2018 Vincent Chambrin
// This file is part of the liblayout project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "stringbox.h"

#include <QFontMetricsF>

StringBox::StringBox(const QString & t, const QFont & f)
  : text(t)
  , font(f)
{
  QFontMetricsF metrics{ f };
  const QRectF rect = metrics.boundingRect(text);
  width_ = rect.width();
  height_ = -rect.top();
  depth_ = rect.bottom();
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

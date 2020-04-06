// Copyright (C) 2019-2020 Vincent Chambrin
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

  reset(-rect.top(), rect.bottom(), rect.width());
}

StringBox::StringBox(const QString & t, const QFont & f, tex::BoxMetrics metrics)
  : tex::Box(metrics), 
    text(t),
    font(f)
{

}

std::shared_ptr<StringBox> stringbox(const QString & text, const QFont & f)
{
  return std::make_shared<StringBox>(text, f);
}

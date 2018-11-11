// Copyright (C) 2018 Vincent Chambrin
// This file is part of the liblayout project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBLAYOUT_EXAMPLES_TEXTPAINTER_H
#define LIBLAYOUT_EXAMPLES_TEXTPAINTER_H

#include "stringbox.h"

#include <tex/layoutreader.h>

#include <QPainter>

struct TextPainter : tex::LayoutReader
{
  QPainter *painter;

  TextPainter(QPainter *p)
    : painter(p)
  {

  }

  static QRectF getRect(tex::Pos pos, const std::shared_ptr<tex::Box> & box)
  {
    QPointF topLeft{ pos.x, pos.y };
    QSizeF size{ box->width(), box->totalHeight() };
    return QRectF{ topLeft, size };
  }

  void operator()(const std::shared_ptr<tex::Box> & box, tex::Pos pos)
  {
    if (!box->is<StringBox>())
      return;

    auto stringbox = std::static_pointer_cast<StringBox>(box);
    painter->setFont(stringbox->font);
    painter->drawText(getRect(pos, box), stringbox->text);
  }
};

#endif // LIBLAYOUT_EXAMPLES_TEXTPAINTER_H

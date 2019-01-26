// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_EXAMPLES_TEXTPAINTER_H
#define LIBTYPESET_EXAMPLES_TEXTPAINTER_H

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

  static QPointF getReferencePoint(tex::Pos pos, const std::shared_ptr<tex::Box> & box)
  {
    return QPointF(pos.x, pos.y + box->height());
  }

  void operator()(const std::shared_ptr<tex::Box> & box, tex::Pos pos)
  {
    if (!box->is<StringBox>())
      return;

    auto stringbox = std::static_pointer_cast<StringBox>(box);
    painter->setFont(stringbox->font);
    //painter->drawText(getRect(pos, box), stringbox->text);
    painter->drawText(getReferencePoint(pos, box), stringbox->text);
  }


  void operator()(const std::shared_ptr<tex::Rule> & rule, tex::Pos pos)
  {
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::black);
    painter->drawRect(getRect(pos, rule));
    painter->restore();
  }
};

#endif // LIBTYPESET_EXAMPLES_TEXTPAINTER_H

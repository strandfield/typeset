// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "renderwidget.h"

#include "typeset-engine.h"

#include <tex/charbox.h>
#include <tex/layoutreader.h>

#include <QPainter>

// borrowed from the examples
struct TextPainter : tex::LayoutReader
{
  QPainter* painter;

  TextPainter(QPainter* p)
    : painter(p)
  {

  }

  static QRectF getRect(tex::Pos pos, const std::shared_ptr<tex::Box>& box)
  {
    QPointF topLeft{ pos.x, pos.y - box->height() };
    QSizeF size{ box->width(), box->totalHeight() };
    return QRectF{ topLeft, size };
  }

  inline static QPointF getReferencePoint(tex::Pos pos)
  {
    return QPointF(pos.x, pos.y);
  }

  void operator()(const std::shared_ptr<tex::Box>& box, tex::Pos pos)
  {
    if (!box->isCharacterBox())
      return;

    CharBox* cbox = static_cast<CharBox*>(box.get());
    painter->setFont(cbox->qfont);
    //painter->drawText(getRect(pos, box), stringbox->text);
    painter->drawText(getReferencePoint(pos), cbox->text);
  }


  void operator()(const std::shared_ptr<tex::Rule>& rule, tex::Pos pos)
  {
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::black);
    painter->drawRect(getRect(pos, rule));
    painter->restore();
  }
};

void RenderWidget::setBox(std::shared_ptr<tex::Box> box)
{
  m_box = box;
  update();
}

void RenderWidget::paintEvent(QPaintEvent* ev)
{
  QPainter p{ this };

  if (m_box != nullptr)
  {
    TextPainter painter{ &p };

    const float x = (width() - m_box->width()) * 0.5f;
    const float y = (height() - m_box->totalHeight()) * 0.5f;

    tex::read(painter, m_box, { x, y });
  }
}

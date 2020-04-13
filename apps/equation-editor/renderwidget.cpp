// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "renderwidget.h"

#include "typeset-engine.h"

#include <tex/charbox.h>
#include <tex/layoutreader.h>

#include <QBrush>
#include <QPainter>

// borrowed from the examples
struct TextPainter : tex::LayoutReader
{
  QPainter* painter;
  bool draw_chars = true;
  bool draw_char_bbox = false;
  bool draw_listbox = false;

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
    {
      if (draw_listbox)
      {
        if (box->is<tex::HBox>())
        {
          painter->save();
          painter->setBrush(Qt::NoBrush);
          painter->setPen(QPen(Qt::red));
          QRectF rect = getRect(pos, box);
          painter->drawRect(rect);
          painter->restore();
        }
        else if (box->is<tex::VBox>())
        {
          painter->save();
          painter->setBrush(Qt::NoBrush);
          painter->setPen(QPen(Qt::green));
          QRectF rect = getRect(pos, box);
          painter->drawRect(rect);
          painter->restore();
        }
      }

      return;
    }

    CharBox* cbox = static_cast<CharBox*>(box.get());

    if (draw_char_bbox)
    {
      painter->save();
      painter->setBrush(Qt::NoBrush);
      painter->setPen(QPen(Qt::blue));
      QRectF rect = getRect(pos, box);
      painter->drawRect(rect);
      painter->restore();
    }

    if (draw_chars)
    {
      painter->save();
      painter->setFont(cbox->qfont);
      //painter->drawText(getRect(pos, box), stringbox->text);
      painter->drawText(getReferencePoint(pos), cbox->text);
      painter->restore();
    }
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

void RenderWidget::setDrawChars(bool on)
{
  if (m_draw_chars != on)
  {
    m_draw_chars = on;
    update();
  }
}

void RenderWidget::setDrawCharBoxes(bool on)
{
  if (m_draw_char_bbox != on)
  {
    m_draw_char_bbox = on;
    update();
  }
}

void RenderWidget::setDrawListBox(bool on)
{
  if (m_draw_listbox != on)
  {
    m_draw_listbox = on;
    update();
  }
}

void RenderWidget::paintEvent(QPaintEvent* ev)
{
  QPainter p{ this };

  p.save();
  p.setBrush(QBrush(Qt::white));
  p.drawRect(this->rect());
  p.restore();

  if (m_box != nullptr)
  {
    TextPainter painter{ &p };
    painter.draw_chars = m_draw_chars;
    painter.draw_char_bbox = m_draw_char_bbox;
    painter.draw_listbox = m_draw_listbox;

    const float x = (width() - m_box->width()) * 0.5f;
    const float y = (height() - m_box->totalHeight()) * 0.5f;

    tex::read(painter, m_box, { x, y });
  }
}

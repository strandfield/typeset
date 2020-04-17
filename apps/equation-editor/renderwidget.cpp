// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "renderwidget.h"

#include "typeset-engine.h"

#include <tex/charbox.h>
#include <tex/hbox.h>
#include <tex/vbox.h>

#include <QBrush>
#include <QPainter>

void EquationEditorRenderWidget::setDrawChars(bool on)
{
  if (m_draw_chars != on)
  {
    m_draw_chars = on;
    update();
  }
}

void EquationEditorRenderWidget::setDrawCharBoxes(bool on)
{
  if (m_draw_char_bbox != on)
  {
    m_draw_char_bbox = on;
    update();
  }
}

void EquationEditorRenderWidget::setDrawListBox(bool on)
{
  if (m_draw_listbox != on)
  {
    m_draw_listbox = on;
    update();
  }
}

void EquationEditorRenderWidget::setDrawBaselines(bool on)
{
  if (m_draw_baselines != on)
  {
    m_draw_baselines = on;
    update();
  }
}

void EquationEditorRenderWidget::paint(QPainter& painter, const std::shared_ptr<tex::Box>& box, const QPointF& pos)
{
  if (!box->isCharacterBox())
  {
    if (m_draw_listbox)
    {
      if (box->is<tex::HBox>())
      {
        painter.save();
        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen(Qt::red));
        QRectF rect = getRect(pos, *box);
        painter.drawRect(rect);
        painter.restore();
      }
      else if (box->is<tex::VBox>())
      {
        painter.save();
        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen(Qt::green));
        QRectF rect = getRect(pos, *box);
        painter.drawRect(rect);
        painter.restore();
      }
    }

    return;
  }

  CharBox* cbox = static_cast<CharBox*>(box.get());

  if (m_draw_char_bbox)
  {
    painter.save();
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(Qt::blue));
    QRectF rect = getRect(pos, *box);
    painter.drawRect(rect);
    painter.restore();
  }

  if (m_draw_baselines)
  {
    painter.save();
    painter.setPen(QPen(Qt::darkMagenta));
    QRectF rect = getRect(pos, *box);
    painter.drawLine(pos, pos + QPointF(rect.width(), 0));
    painter.restore();
  }

  if (m_draw_chars)
  {
    RenderWidget::paint(painter, box, pos);
  }
}

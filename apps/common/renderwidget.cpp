// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "renderwidget.h"

#include "qt-typeset-engine.h"

#include <tex/charbox.h>
#include <tex/layoutreader.h>

#include <QBrush>
#include <QGlyphRun>
#include <QPainter>

// borrowed from the examples
class LayoutVisitor : tex::LayoutReader
{
public:
  RenderWidget* widget;
  QPainter* painter;

  LayoutVisitor(RenderWidget*w, QPainter* p)
    : widget(w), painter(p)
  {

  }

  void operator()(const std::shared_ptr<tex::Box>& box, tex::Pos pos)
  {
    widget->paint(*painter, box, QPointF{ pos.x, pos.y });
  }

  void operator()(const std::shared_ptr<tex::Rule>& rule, tex::Pos pos)
  {
    widget->paint(*painter, rule, QPointF{ pos.x, pos.y });
  }
};

RenderWidget::RenderWidget(QWidget* parent)
  : QWidget(parent)
{
  m_margins = QMargins(10, 10, 10, 10);
}

void RenderWidget::center(bool on)
{
  if (m_center != on)
  {
    m_center = on;
    update();
  }
}

bool RenderWidget::centered() const
{
  return m_center;
}

void RenderWidget::setMargins(QMargins margins)
{
  m_margins = margins;
  update();
}

const QMargins& RenderWidget::margins() const
{
  return m_margins;
}

void RenderWidget::setBox(std::shared_ptr<tex::Box> box)
{
  m_box = box;
  update();
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
    visit(p, m_box);
  }
}

void RenderWidget::visit(QPainter& painter, std::shared_ptr<tex::Box> box)
{
  LayoutVisitor visitor{ this, &painter };

  float x = margins().left();
  float y = margins().top();

  if (centered())
  {
    x += (width() - margins().left() - margins().right() - m_box->width()) * 0.5f;
    y += (height() - margins().top() - margins().bottom() - m_box->totalHeight()) * 0.5f + m_box->height();
  }
  else
  {
    y += m_box->height();
  }

  tex::read(visitor, m_box, { x, y });
}

QRectF RenderWidget::getRect(const QPointF& pos, const tex::Box& box)
{
  QPointF topLeft{ pos.x(), pos.y() - box.height() };
  QSizeF size{ box.width(), box.totalHeight() };
  return QRectF{ topLeft, size };
}

void RenderWidget::paint(QPainter& painter, const std::shared_ptr<tex::Box>& box, const QPointF& pos)
{
  if (!box->isCharacterBox())
    return;

  CharBox* cbox = static_cast<CharBox*>(box.get());

  painter.save();
  painter.setFont(cbox->qfont);

  if (!cbox->deformed)
  {
    painter.drawText(pos, cbox->text);
  }
  else
  {
    painter.translate(pos);
    const float sx = cbox->metrics().width / cbox->oribox.width;
    const float sy = (cbox->metrics().height + cbox->metrics().depth) / (cbox->oribox.height + cbox->oribox.depth);
    painter.scale(sx, sy);

    QGlyphRun run;
    run.setRawFont(cbox->rawfont);
    QVector<quint32> indices = cbox->rawfont.glyphIndexesForString(cbox->text);
    run.setGlyphIndexes(indices);
    run.setPositions({ { 0.f, 0.f } });
    painter.drawGlyphRun(QPointF{ 0.f, 0.f }, run);
  }


  painter.restore();
}

void RenderWidget::paint(QPainter& painter, const std::shared_ptr<tex::Rule>& rule, const QPointF& pos)
{
  painter.save();
  painter.setPen(Qt::NoPen);
  painter.setBrush(Qt::black);
  painter.drawRect(getRect(pos, *rule));
  painter.restore();
}

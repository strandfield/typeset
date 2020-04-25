// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "pagewidget.h"

#include <QBrush>
#include <QPainter>

#include <QScrollBar>

PageWidget::PageWidget(QWidget* parent)
  : QWidget(parent)
{
  m_renderwidget = new RenderWidget(this);
  m_renderwidget->setFixedWidth(m_pagewidth);

  m_vertical_scrollbar = new QScrollBar(Qt::Vertical, this);
  m_vertical_scrollbar->setVisible(false);

  connect(m_vertical_scrollbar, &QScrollBar::valueChanged, this, &PageWidget::updateLayout);
}

void PageWidget::setPageWidth(int w)
{
  m_pagewidth = w;
  m_renderwidget->setFixedWidth(w);
}

int PageWidget::pagewidth() const
{
  return m_pagewidth;
}

int PageWidget::hsize() const
{
  return m_pagewidth - margins().left() - margins().right();
}

void PageWidget::setMargins(QMargins margins)
{
  m_renderwidget->setMargins(margins);
}

const QMargins& PageWidget::margins() const
{
  return m_renderwidget->margins();
}

void PageWidget::setBox(std::shared_ptr<tex::Box> box)
{
  m_renderwidget->setBox(box);

  int h = box->totalHeight() + margins().top() + margins().bottom();
  h = h > 600 ? h : 600;

  m_renderwidget->setFixedHeight(h);
  updateLayout();
}

void PageWidget::paintEvent(QPaintEvent* ev)
{
  QPainter p{ this };

  p.save();
  p.setBrush(QBrush(Qt::gray));
  p.drawRect(this->rect());
  p.restore();
}

void PageWidget::resizeEvent(QResizeEvent* ev)
{
  QWidget::resizeEvent(ev);
  
  m_vertical_scrollbar->move(width() - m_vertical_scrollbar->width(), 0);
  m_vertical_scrollbar->setFixedHeight(height());

  updateLayout();
}

void PageWidget::updateLayout()
{
  if (m_renderwidget->height() > height() + 20)
  {
    if (!m_vertical_scrollbar->isVisible())
    {
      m_vertical_scrollbar->move(width() - m_vertical_scrollbar->width(), 0);
      m_vertical_scrollbar->setFixedHeight(height());
    }

    m_vertical_scrollbar->setVisible(true);

    m_vertical_scrollbar->setRange(-10, m_renderwidget->height() + 10 - height());

    int x = (width() - m_renderwidget->width()) * 0.5;
    int y = -m_vertical_scrollbar->value();
    m_renderwidget->move(x, y);
  }
  else
  {
    m_renderwidget->move(QPoint(width() - m_renderwidget->width(), height() - m_renderwidget->height()) * 0.5);
    m_vertical_scrollbar->setVisible(false);
  }
}

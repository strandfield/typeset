// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "linebreaks-viewer-render-widget.h"

#include "tex/hbox.h"
#include "tex/linebreaks.h"

#include <QPainter>
#include <QPen>

void LinebreaksViewerRenderWidget::paint(QPainter& painter, const std::shared_ptr<tex::Box>& box, const QPointF& pos)
{
  RenderWidget::paint(painter, box, pos);

  if (!show_ratios)
    return;

  if (box->isHBox())
  {
    painter.save();

    const auto& hbox = box->as<tex::HBox>();

    float ratio = hbox.glueRatio();
    
    if (hbox.glueOrder() == tex::GlueOrder::Normal)
    {
      switch (tex::Paragraph::getFitnessClass(ratio))
      {
      case tex::FitnessClass::Tight:
        painter.setPen(QPen(Qt::red));
        break;
      case tex::FitnessClass::Loose:
        painter.setPen(QPen(Qt::blue));
        break;
      case tex::FitnessClass::VeryLoose:
        painter.setPen(QPen(Qt::red));
        break;
      default:
        break;
      }
    }

    painter.drawText(getRect(pos, *box).topRight() + QPointF{ 10.f, 10.f }, QString::number(ratio));

    painter.restore();
  }
}
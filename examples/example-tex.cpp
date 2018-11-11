// Copyright (C) 2018 Vincent Chambrin
// This file is part of the liblayout project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "example-tex.h"

#include "stringbox.h"

#include "tex/layoutreader.h"

#include <QPainter>
#include <QPaintEvent>

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

TeXWidget::TeXWidget(QWidget *parent)
  : QWidget(parent)
{
  /// See TeXbook Chapter 11 Boxes page 66
  // \hbox{T\kern-.1667em\lower.5ex\hbox{E}\kern-.125em X}

  QFont font = QFont{ "Times New Roman" };
  font.setPointSize(48);

  QFontMetricsF metrics{ font };
  const float xHeight = metrics.xHeight();
  const float em = metrics.width(QChar('M'));

  auto T = std::make_shared<StringBox>("T", font);
  auto E = std::make_shared<StringBox>("E", font);
  auto X = std::make_shared<StringBox>("X", font);

  auto boxE = tex::hbox({ E });
  tex::lower(boxE, 0.5f * xHeight);

  layout_ = tex::hbox({ T, tex::kern(-0.1667 * em), boxE, tex::kern(-0.125 * em), X });
}

void TeXWidget::paintEvent(QPaintEvent *e)
{
  QPainter p{ this };

  TextPainter painter{ &p };

  const float x = (width() - layout_->width()) * 0.5f;
  const float y = (height() - layout_->totalHeight()) * 0.5f;

  tex::read(painter, layout_, { x, y });
}
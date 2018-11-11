// Copyright (C) 2018 Vincent Chambrin
// This file is part of the liblayout project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBLAYOUT_EXAMPLES_TEX_H
#define LIBLAYOUT_EXAMPLES_TEX_H

#include <QWidget>

namespace tex
{
class Box;
} // namespace tex

class TeXWidget : public QWidget
{
  Q_OBJECT
public:
  TeXWidget(QWidget *parent = nullptr);
  ~TeXWidget() = default;

protected:
  void paintEvent(QPaintEvent *e) override;

private:
  std::shared_ptr<tex::Box> layout_;
};

#endif // LIBLAYOUT_EXAMPLES_TEX_H

// Copyright (C) 2018 Vincent Chambrin
// This file is part of the liblayout project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBLAYOUT_EXAMPLES_MATH_H
#define LIBLAYOUT_EXAMPLES_MATH_H

#include "example.h"

namespace tex
{
class Box;
} // namespace tex

class QPlainTextEdit;

class MathTextWidget : public Example
{
  Q_OBJECT
public:
  MathTextWidget(QWidget *parent = nullptr);
  ~MathTextWidget() = default;

  QString getName() const override;
  QWidget* getController() override;

public Q_SLOTS:
  void recomputeLayout(int pagewidth = -1);

protected:
  void paintEvent(QPaintEvent *e) override;

private:
  std::shared_ptr<tex::Box> layout_;
};

#endif // LIBLAYOUT_EXAMPLES_MATH_H

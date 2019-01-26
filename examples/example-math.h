// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_EXAMPLES_MATH_H
#define LIBTYPESET_EXAMPLES_MATH_H

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

#endif // LIBTYPESET_EXAMPLES_MATH_H

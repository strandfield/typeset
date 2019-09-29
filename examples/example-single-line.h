// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_EXAMPLES_SINGLELINE_H
#define LIBTYPESET_EXAMPLES_SINGLELINE_H

#include "example.h"

#include <memory>

namespace tex
{
class HBox;
} // namespace tex

class SingleLineTextWidget : public Example
{
  Q_OBJECT
public:
  SingleLineTextWidget(QWidget *parent = nullptr);
  ~SingleLineTextWidget() = default;

  QString getName() const override;
  QWidget* getController() override;

public Q_SLOTS:
  void constructLayout();
  void recomputeLayout(int pagewidth = -1);

protected:
  void paintEvent(QPaintEvent *e) override;

private:
  std::shared_ptr<tex::HBox> layout_;
  int pagewidth_;
};

#endif // LIBTYPESET_EXAMPLES_SINGLELINE_H

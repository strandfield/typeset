// Copyright (C) 2018 Vincent Chambrin
// This file is part of the liblayout project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBLAYOUT_EXAMPLES_SINGLELINE_H
#define LIBLAYOUT_EXAMPLES_SINGLELINE_H

#include "example.h"

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

#endif // LIBLAYOUT_EXAMPLES_SINGLELINE_H

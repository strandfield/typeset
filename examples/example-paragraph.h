// Copyright (C) 2018 Vincent Chambrin
// This file is part of the liblayout project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBLAYOUT_EXAMPLES_PARAGRAPH_H
#define LIBLAYOUT_EXAMPLES_PARAGRAPH_H

#include "example.h"

namespace tex
{
class Box;
} // namespace tex

class QPlainTextEdit;

class ParagraphTextWidget : public Example
{
  Q_OBJECT
public:
  ParagraphTextWidget(QWidget *parent = nullptr);
  ~ParagraphTextWidget() = default;

  QString getName() const override;
  QWidget* getController() override;

public Q_SLOTS:
  void recomputeLayout(int pagewidth = -1);

protected:
  void paintEvent(QPaintEvent *e) override;

private:
  std::shared_ptr<tex::Box> layout_;
  int pagewidth_;
  QPlainTextEdit *text_;
};

#endif // LIBLAYOUT_EXAMPLES_PARAGRAPH_H

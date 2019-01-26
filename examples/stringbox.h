// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_EXAMPLES_STRINGBOX_H
#define LIBTYPESET_EXAMPLES_STRINGBOX_H

#include <tex/box.h>

#include <QString>
#include <QFont>

class StringBox : public tex::Box
{
public:
  QString text;
  QFont font;

  StringBox(const QString & t, const QFont & f = QFont());
  StringBox(const QString & t, const QFont & f, tex::BoxMetrics metrics);
  ~StringBox() = default;

  float height() const override;
  float depth() const override;
  float width() const override;

private:
  float height_;
  float depth_;
  float width_;
};

std::shared_ptr<StringBox> stringbox(const QString & text, const QFont & f = QFont());

#endif // LIBTYPESET_EXAMPLES_STRINGBOX_H

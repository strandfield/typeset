// Copyright (C) 2018 Vincent Chambrin
// This file is part of the liblayout project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBLAYOUT_EXAMPLES_STRINGBOX_H
#define LIBLAYOUT_EXAMPLES_STRINGBOX_H

#include <tex/box.h>

#include <QString>
#include <QFont>

class StringBox : public tex::Box
{
public:
  QString text;
  QFont font;

  StringBox(const QString & t, const QFont & f = QFont());
  ~StringBox() = default;

  float height() const override;
  float depth() const override;
  float width() const override;

private:
  float height_;
  float depth_;
  float width_;
};

#endif // LIBLAYOUT_EXAMPLES_STRINGBOX_H

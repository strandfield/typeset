// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_EXAMPLES_EXAMPLE_H
#define LIBTYPESET_EXAMPLES_EXAMPLE_H

#include <QWidget>

class Example : public QWidget
{
  Q_OBJECT
public:
  Example(QWidget *parent = nullptr);

  virtual QString getName() const = 0;
  virtual QWidget* getController();
};

#endif // LIBTYPESET_EXAMPLES_EXAMPLE_H

// Copyright (C) 2018 Vincent Chambrin
// This file is part of the liblayout project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBLAYOUT_EXAMPLES_EXAMPLE_H
#define LIBLAYOUT_EXAMPLES_EXAMPLE_H

#include <QWidget>

class Example : public QWidget
{
  Q_OBJECT
public:
  Example(QWidget *parent = nullptr);

  virtual QString getName() const = 0;
  virtual QWidget* getController();
};

#endif // LIBLAYOUT_EXAMPLES_EXAMPLE_H

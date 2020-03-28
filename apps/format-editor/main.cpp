// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  MainWindow w;
  w.show();

  return app.exec();
}
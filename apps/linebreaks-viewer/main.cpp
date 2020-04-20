// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "linebreaks-viewer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  LinebreaksViewerWindow w;
  w.show();

  return app.exec();
}
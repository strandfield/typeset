// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_FORMATEDITOR_MAINWINDOW_H
#define LIBTYPESET_FORMATEDITOR_MAINWINDOW_H

#include <QMainWindow>

class Editor;

class MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit MainWindow();
  ~MainWindow() = default;

protected Q_SLOTS:
  void exportFormat();
  void loadFormat();

private:
  Editor* m_editor;
};

#endif // LIBTYPESET_FORMATEDITOR_MAINWINDOW_H

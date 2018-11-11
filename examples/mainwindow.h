// Copyright (C) 2018 Vincent Chambrin
// This file is part of the liblayout project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBLAYOUT_EXAMPLES_MAINWINDOW_H
#define LIBLAYOUT_EXAMPLES_MAINWINDOW_H

#include <QMainWindow>

class QSettings;
class QTabWidget;

class MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit MainWindow();
  ~MainWindow() = default;

protected Q_SLOTS:

protected:
  void showEvent(QShowEvent *e);
  void closeEvent(QCloseEvent *e);

private:
  QSettings *mSettings;
  QTabWidget *mTabWidget;
};

#endif // LIBLAYOUT_EXAMPLES_MAINWINDOW_H

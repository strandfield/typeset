// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_EXAMPLES_MAINWINDOW_H
#define LIBTYPESET_EXAMPLES_MAINWINDOW_H

#include <QMainWindow>

class Example;

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
  void addExample(Example *ex);

protected:
  void showEvent(QShowEvent *e);
  void closeEvent(QCloseEvent *e);

private:
  QSettings *mSettings;
  QTabWidget *mTabWidget;
};

#endif // LIBTYPESET_EXAMPLES_MAINWINDOW_H

// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_PAGEEDITOR_MAINWINDOW_H
#define LIBTYPESET_PAGEEDITOR_MAINWINDOW_H

#include <QMainWindow>

class QLabel;
class QPlainTextEdit;

class RenderWidget;
class TypesetEngine;

class MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit MainWindow();
  ~MainWindow();

protected Q_SLOTS:
  void onTextChanged();

protected:
  void processText();

private:
  std::shared_ptr<TypesetEngine> m_engine;
  RenderWidget* m_renderwidget;
  QPlainTextEdit* m_textedit;
  QLabel* m_status_widget;
};

#endif // LIBTYPESET_PAGEEDITOR_MAINWINDOW_H

// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_EQEDITOR_MAINWINDOW_H
#define LIBTYPESET_EQEDITOR_MAINWINDOW_H

#include <QWidget>

class QComboBox;
class QPlainTextEdit;
class QSpinBox;

class RenderWidget;

class MainWindow : public QWidget
{
  Q_OBJECT
public:
  explicit MainWindow();
  ~MainWindow() = default;

protected:
  QWidget* createSettingsWidget();

private:
  RenderWidget* m_renderwidget;
  QPlainTextEdit* m_textedit;
};

#endif // LIBTYPESET_EQEDITOR_MAINWINDOW_H

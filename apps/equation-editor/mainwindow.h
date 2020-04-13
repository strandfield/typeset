// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_EQEDITOR_MAINWINDOW_H
#define LIBTYPESET_EQEDITOR_MAINWINDOW_H

#include <QWidget>

class QComboBox;
class QCheckBox;
class QLabel;
class QPlainTextEdit;
class QSpinBox;

class FontTreeWidget;
class RenderWidget;
class TypesetEngine;

class MainWindow : public QWidget
{
  Q_OBJECT
public:
  explicit MainWindow();
  ~MainWindow();

protected:
  QWidget* createSettingsWidget();

protected Q_SLOTS:
  void onTextChanged();
  void onShowOnlyUsedFontDimenChanged();
  void onDrawCharChanged();
  void onDrawCharBoxesChanged();
  void onDrawListBoxChanged();

protected:
  void processText();

private:
  std::shared_ptr<TypesetEngine> m_engine;
  QCheckBox* m_drawchars_checkbox;
  QCheckBox* m_drawcharbox_checkbox;
  QCheckBox* m_drawlistbox_checkbox;
  QCheckBox* m_showonlyused_checkbox;
  FontTreeWidget* m_font_treewidget;
  RenderWidget* m_renderwidget;
  QPlainTextEdit* m_textedit;
  QLabel* m_report_widget;
};

#endif // LIBTYPESET_EQEDITOR_MAINWINDOW_H

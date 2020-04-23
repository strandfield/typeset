// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_LINEBREAKSVIEWER_WINDOW_H
#define LIBTYPESET_LINEBREAKSVIEWER_WINDOW_H

#include <QMainWindow>

#include "common/renderwidget.h"
#include "common/qt-typeset-engine.h"

#include "tex/listbox.h"
#include "tex/linebreaks.h"
#include "tex/parshape.h"

class QComboBox;
class QCheckBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QPlainTextEdit;
class QPushButton;
class QSpinBox;

class LinebreaksViewerRenderWidget;

class LinebreaksViewerWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit LinebreaksViewerWindow();
  ~LinebreaksViewerWindow();

protected:
  QWidget* createSettingsWidget();


protected Q_SLOTS:
  void onTextChanged();
  void onDrawRatioChanged();
  void onParshapeChanged();
  void onParameterChanged();
  void resetParameters();
  void onSelectedBreakpointChanged();

protected:
  tex::Parshape parseParshape() const;
  void processText();
  void setup(tex::Paragraph& linebreaker);

private:
  std::shared_ptr<TypesetEngine> m_engine;
  tex::List m_list;
  tex::Parshape m_parshape;
  std::vector<std::shared_ptr<tex::Paragraph::Breakpoint>> m_breakpoints;
  QCheckBox* m_draw_ratios;
  QSpinBox* m_tolerance_spinbox;
  QSpinBox* m_adjdemerits_spinbox;
  QSpinBox* m_linepenalty_spinbox;
  QLineEdit* m_parshape_lineedit;
  QPushButton* m_reset_button;
  LinebreaksViewerRenderWidget* m_renderwidget;
  QPlainTextEdit* m_textedit;
  QGroupBox* m_report_groupbox;
  QLabel* m_report_widget;
  QLabel* m_nblinebreaks_label;
  QSpinBox* m_linebreaks_spinbox;
  QLabel* m_demerits_label;
};

#endif // LIBTYPESET_LINEBREAKSVIEWER_WINDOW_H

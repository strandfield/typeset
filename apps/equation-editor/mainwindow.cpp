// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "mainwindow.h"

#include "renderwidget.h"

#include <QPlainTextEdit>
#include <QSpinBox>

#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QFrame>
#include <QGroupBox>
#include <QSplitter>

MainWindow::MainWindow()
{
  setWindowTitle("Equation Editor");

  QHBoxLayout* layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  
  QSplitter* splitter = new QSplitter(Qt::Vertical);
  m_renderwidget = new RenderWidget;
  m_textedit = new QPlainTextEdit;
  splitter->addWidget(m_renderwidget);
  splitter->addWidget(m_textedit);

  layout->addWidget(splitter, 1);

  QFrame* line = new QFrame;
  line->setFixedWidth(2);
  line->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
  line->setStyleSheet(QString("background-color: #c0c0c0;"));
  layout->addWidget(line);

  layout->addWidget(createSettingsWidget());
}

QWidget* MainWindow::createSettingsWidget()
{
  QWidget* w = new QWidget;

  QVBoxLayout* layout = new QVBoxLayout(w);

  {
    QGroupBox* gb = new QGroupBox("Font parameters");

    QFormLayout* form = new QFormLayout(gb);
    form->addRow("x-height", new QSpinBox());

    layout->addWidget(gb);
  }

  layout->addStretch();

  return w;
}
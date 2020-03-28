// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "mainwindow.h"

#include "editor.h"

#include <QMenuBar>

#include <QFileDialog>

MainWindow::MainWindow()
{
  setWindowTitle("Format Editor");

  m_editor = new Editor(this);

  setCentralWidget(m_editor);

  QAction* open_action = menuBar()->addAction("Open");
  QAction* export_action = menuBar()->addAction("Export");

  connect(open_action, &QAction::triggered, this, &MainWindow::loadFormat);
  connect(export_action, &QAction::triggered, this, &MainWindow::exportFormat);
}

void MainWindow::exportFormat()
{
  QString path = QFileDialog::getSaveFileName(this);

  if (path.isEmpty())
    return;

  std::string data = m_editor->computeFormat();

  QFile file{ path };

  if (!file.open(QIODevice::WriteOnly))
    return;

  file.write(data.data());

  file.close();
}

void MainWindow::loadFormat()
{
  QString path = QFileDialog::getOpenFileName(this);

  if (path.isEmpty())
    return;


  QFile file{ path };

  if (!file.open(QIODevice::ReadOnly))
    return;

  std::string content = file.readAll().constData();

  file.close();

  m_editor->loadFormat(content);
}



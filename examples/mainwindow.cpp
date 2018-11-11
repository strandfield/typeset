// Copyright (C) 2018 Vincent Chambrin
// This file is part of the liblayout project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "mainwindow.h"

#include "example-tex.h"

#include <QMenuBar>
#include <QMessageBox>
#include <QSettings>
#include <QTabWidget>

MainWindow::MainWindow()
{
  setWindowTitle("liblayout examples");

  mSettings = new QSettings("settings.ini", QSettings::IniFormat, this);

  mTabWidget = new QTabWidget();
  mTabWidget->addTab(new TeXWidget(), "TeX");

  setCentralWidget(mTabWidget);
}

void MainWindow::showEvent(QShowEvent *e)
{
  if (mSettings->contains("geometry"))
    restoreGeometry(mSettings->value("geometry").toByteArray());

  if (mSettings->contains("windowState"))
    restoreState(mSettings->value("windowState").toByteArray());

  QMainWindow::showEvent(e);
}

void MainWindow::closeEvent(QCloseEvent *e)
{
  mSettings->setValue("geometry", saveGeometry());
  mSettings->setValue("windowState", saveState());
  QMainWindow::closeEvent(e);
}
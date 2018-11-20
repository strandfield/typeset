// Copyright (C) 2018 Vincent Chambrin
// This file is part of the liblayout project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "mainwindow.h"

#include "example.h"
#include "example-tex.h"
#include "example-single-line.h"
#include "example-paragraph.h"

#include <QHBoxLayout>
#include <QMenuBar>
#include <QMessageBox>
#include <QSettings>
#include <QTabWidget>
#include <QVBoxLayout>

MainWindow::MainWindow()
{
  setWindowTitle("liblayout examples");

  mSettings = new QSettings("settings.ini", QSettings::IniFormat, this);

  mTabWidget = new QTabWidget();
  mTabWidget->addTab(new TeXWidget(), "TeX");

  setCentralWidget(mTabWidget);

  addExample(new SingleLineTextWidget());
  addExample(new ParagraphTextWidget());
}

void MainWindow::addExample(Example *ex)
{
  QWidget *controller = ex->getController();
  if (controller == nullptr)
  {
    mTabWidget->addTab(ex, ex->getName());
    return;
  }

  QWidget *widget = new QWidget();
  QHBoxLayout *hbox = new QHBoxLayout(widget);

  hbox->addWidget(ex, 1);

  QVBoxLayout *sublayout = new QVBoxLayout();
  sublayout->addWidget(controller);
  sublayout->addStretch();
  hbox->addLayout(sublayout);

  mTabWidget->addTab(widget, ex->getName());
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
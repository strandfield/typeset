// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "mainwindow.h"

#include "common/renderwidget.h"
#include "common/qt-typeset-engine.h"

#include "typesetting-machine.h"

#include <QLabel>
#include <QPlainTextEdit>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QSplitter>

#include <chrono>

#include <QDebug>

static QWidget* vbox(std::vector<QWidget*> widgets, int spacing = 0)
{
  QWidget* result = new QWidget;
  QVBoxLayout* layout = new QVBoxLayout(result);
  layout->setSpacing(spacing);
  layout->setContentsMargins(0, 0, 0, 0);
  
  for (QWidget* w : widgets)
  {
    layout->addWidget(w);
  }

  return result;
}

MainWindow::MainWindow()
{
  setWindowTitle("Page Editor");

  m_engine = std::make_shared<TypesetEngine>();

  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  QSplitter *horizontal_splitter = new QSplitter(Qt::Horizontal);

  m_textedit = new QPlainTextEdit;
  horizontal_splitter->addWidget(m_textedit);

  m_renderwidget = new RenderWidget;
  horizontal_splitter->addWidget(m_renderwidget);

  layout->addWidget(horizontal_splitter, 1);

  m_status_widget = new QLabel;
  layout->addWidget(m_status_widget);

  connect(m_textedit, &QPlainTextEdit::textChanged, this, &MainWindow::onTextChanged);
}

MainWindow::~MainWindow()
{

}

void MainWindow::onTextChanged()
{
  try 
  {
    processText();
  }
  catch (const TypesettingException & ex)
  {
    m_status_widget->setText(QString::number(ex.line) + ":" + QString::number(ex.col) + ": " + ex.what());
  }
  catch (const std::runtime_error & ex)
  {
    m_status_widget->setText(ex.what());
  }
}

static double duration_msec(std::chrono::duration<double> diff)
{
  return diff.count() * 1000;
}

void MainWindow::processText()
{
  std::string text = m_textedit->toPlainText().toStdString();

  auto start = std::chrono::high_resolution_clock::now();

  if (text.empty())
    return;

  TypesettingMachine machine{ m_engine, tex::Font(0) };
  auto box = machine.typeset(text);

  auto end = std::chrono::high_resolution_clock::now();

  m_renderwidget->setBox(box);

  QString report = "Total: " + QString::number(duration_msec(end - start));
  m_status_widget->setText(report);
}

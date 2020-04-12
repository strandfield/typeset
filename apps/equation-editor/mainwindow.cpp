// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "mainwindow.h"

#include "typeset-engine.h"
#include "renderwidget.h"

#include "tex/lexer.h"
#include "tex/parsing/mathparserfrontend.h"
#include "tex/math/math-typeset.h"

#include <QPlainTextEdit>
#include <QSpinBox>

#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QFrame>
#include <QGroupBox>
#include <QSplitter>

#include <QDebug>

MainWindow::MainWindow()
{
  setWindowTitle("Equation Editor");

  m_engine = std::make_shared<TypesetEngine>();

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

  connect(m_textedit, &QPlainTextEdit::textChanged, this, &MainWindow::onTextChanged);
}

MainWindow::~MainWindow()
{

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

void MainWindow::onTextChanged()
{
  try 
  {
    processText();
  }
  catch (const std::runtime_error & ex)
  {
    qDebug() << ex.what();
  }
}

void MainWindow::processText()
{
  std::string text = m_textedit->toPlainText().toStdString();

  tex::parsing::Lexer lexer;

  for (char c : text)
  {
    lexer.write(c);
  }

  tex::parsing::MathParserFrontend parser;

  for (const tex::parsing::Token& tok : lexer.output())
  {
    if (tok.isCharacterToken())
    {
      tex::parsing::CharacterToken ctok = tok.characterToken();

      switch (ctok.category)
      {
      case tex::parsing::CharCategory::GroupBegin:
        parser.beginMathList();
        break;
      case tex::parsing::CharCategory::GroupEnd:
        parser.endMathList();
        break;
      case tex::parsing::CharCategory::Superscript:
        parser.beginSuperscript();
        break;
      case tex::parsing::CharCategory::Subscript:
        parser.beginSubscript();
        break;
      case tex::parsing::CharCategory::Letter:
      case tex::parsing::CharCategory::Other:
        parser.writeChar(ctok.value);
        break;
      default:
        break;
      }
    }
    else if (tok.isControlSequence())
    {
      parser.writeControlSequence(tok.controlSequence());
    }
  }

  parser.finish();

  tex::MathTypesetter mathtypesetter{m_engine};
  mathtypesetter.setFonts(m_engine->mathfonts());

  auto hlist = mathtypesetter.mlist2hlist(parser.output());
  auto box = tex::hbox(std::move(hlist));

  m_renderwidget->setBox(box);
}

// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "linebreaks-viewer.h"

#include "linebreaks-viewer-render-widget.h"

#include "tex/hlist.h"
#include "tex/lexer.h"
#include "tex/linebreaks.h"
#include "tex/mathchars.h"
#include "tex/vbox.h"

#include "tex/parsing/glueparser.h"

#include <QAction>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QMenuBar>
#include <QPalette>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QStatusBar>

#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QFrame>
#include <QGroupBox>
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

static float total_height(const tex::BoxMetrics& bm)
{
  return bm.height + bm.depth;
}

LinebreaksViewerWindow::LinebreaksViewerWindow()
{
  setWindowTitle("Linebreaks Viewer");

  m_engine = std::make_shared<TypesetEngine>();
  m_unitsystem.em = m_engine->metrics()->quad(tex::Font(0));
  m_unitsystem.ex = m_engine->metrics()->xHeight(tex::Font(0));
  m_unitsystem.pt = total_height(m_engine->metrics()->metrics('(', tex::Font(0))) / 10.f;

  QWidget* content = new QWidget;

  QHBoxLayout* layout = new QHBoxLayout(content);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  QSplitter *horizontal_splitter = new QSplitter(Qt::Horizontal);
  
  QSplitter* vertical_splitter = new QSplitter(Qt::Vertical);
  m_renderwidget = new LinebreaksViewerRenderWidget;
  vertical_splitter->addWidget(m_renderwidget);

  m_textedit = new QPlainTextEdit;
  vertical_splitter->addWidget(vbox({ m_textedit }));

  horizontal_splitter->addWidget(vertical_splitter);

  horizontal_splitter->addWidget(createSettingsWidget());

  layout->addWidget(horizontal_splitter);

  setCentralWidget(content);

  menuBar()->addAction("lorem ipsum", [this]() {
    m_textedit->setPlainText("Lorem ipsum dolor sit amet, consectetur adipiscing elit. "
      "Aenean eget tempor libero. Sed pulvinar elit libero, a dignissim felis venenatis id. "
      "Interdum et malesuada fames ac ante ipsum primis in faucibus. "
      "Ut placerat turpis vel dolor fermentum congue. Nam vitae justo ut risus placerat finibus. "
      "Pellentesque sit amet iaculis odio. Nullam tempor iaculis augue, a sollicitudin odio convallis vitae. "
      "Nulla laoreet dignissim mi ac bibendum. In convallis nunc sollicitudin magna pharetra, vel vestibulum risus vehicula. "
      "Fusce ut ipsum vitae erat mollis iaculis. Fusce lacinia mauris eget dolor bibendum, sit amet suscipit felis consequat. "
      "Phasellus sagittis orci metus, eleifend blandit nunc porta et. Maecenas ut facilisis libero. ");
    });

  connect(m_textedit, &QPlainTextEdit::textChanged, this, &LinebreaksViewerWindow::onTextChanged);

  onParameterChanged();
}

LinebreaksViewerWindow::~LinebreaksViewerWindow()
{

}

QWidget* LinebreaksViewerWindow::createSettingsWidget()
{
  QWidget* w = new QWidget;

  QVBoxLayout* layout = new QVBoxLayout(w);

  {
    QGroupBox* gb = new QGroupBox("Options");

    QVBoxLayout* gbl = new QVBoxLayout(gb);

    m_draw_ratios = new QCheckBox("Show ratios");
    m_draw_ratios->setChecked(false);
    gbl->addWidget(m_draw_ratios);

    QFormLayout* form = new QFormLayout;

    {
      m_frenchspacing_input = new QCheckBox;
      m_frenchspacing_input->setChecked(false);
      form->addRow("frenchspacing", m_frenchspacing_input);

      m_tolerance_spinbox = new QSpinBox;
      m_tolerance_spinbox->setRange(200, 20000);
      m_tolerance_spinbox->setValue(800);
      form->addRow("tolerance", m_tolerance_spinbox); 

      m_adjdemerits_spinbox = new QSpinBox;
      m_adjdemerits_spinbox->setRange(1000, 100000);
      m_adjdemerits_spinbox->setValue(10000);
      form->addRow("adjdemerits", m_adjdemerits_spinbox);

      m_linepenalty_spinbox = new QSpinBox;
      m_linepenalty_spinbox->setRange(0, 1000);
      m_linepenalty_spinbox->setValue(10);
      form->addRow("linepenalty", m_linepenalty_spinbox);

      m_leftskip_lineedit = new QLineEdit("0pt");
      form->addRow("leftskip", m_leftskip_lineedit);

      m_rightskip_lineedit = new QLineEdit("0pt");
      form->addRow("rightskip", m_rightskip_lineedit);

      m_baselineskip_lineedit = new QLineEdit("12pt");
      form->addRow("baselineskip", m_baselineskip_lineedit);

      m_lineskip_lineedit = new QLineEdit("1pt");
      form->addRow("lineskip", m_lineskip_lineedit);

      m_lineskiplimit_lineedit = new QLineEdit("0pt");
      form->addRow("lineskiplimit", m_lineskiplimit_lineedit);

      m_hangindent_input = new QLineEdit("0pt");
      form->addRow("hangindent", m_hangindent_input);

      m_hangafter_input = new QSpinBox();
      m_hangafter_input->setRange(-100, 100);
      m_hangafter_input->setValue(1);
      form->addRow("hangafter", m_hangafter_input);

      m_parshape_lineedit = new QLineEdit;
      m_parshape_lineedit->setPlaceholderText("enter parshape specifications");
      form->addRow("parshape", m_parshape_lineedit);
    }
    
    gbl->addLayout(form);

    m_reset_button = new QPushButton("Reset");
    gbl->addWidget(m_reset_button);

    layout->addWidget(gb);
  }

  {
    m_report_groupbox = new QGroupBox("Report");

    QVBoxLayout* gbl = new QVBoxLayout(m_report_groupbox);

    m_report_widget = new QLabel;
    gbl->addWidget(m_report_widget);

    m_nblinebreaks_label = new QLabel;
    gbl->addWidget(m_nblinebreaks_label);

    m_linebreaks_spinbox = new QSpinBox;
    gbl->addWidget(m_linebreaks_spinbox);

    m_demerits_label = new QLabel;
    gbl->addWidget(m_demerits_label);

    layout->addWidget(m_report_groupbox);
  }

  layout->addStretch();

  connect(m_draw_ratios, &QCheckBox::toggled, this, &LinebreaksViewerWindow::onDrawRatioChanged);
  connect(m_frenchspacing_input, &QCheckBox::toggled, this, &LinebreaksViewerWindow::onParameterChanged);
  connect(m_tolerance_spinbox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &LinebreaksViewerWindow::onParameterChanged);
  connect(m_adjdemerits_spinbox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &LinebreaksViewerWindow::onParameterChanged);
  connect(m_linepenalty_spinbox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &LinebreaksViewerWindow::onParameterChanged);
  connect(m_leftskip_lineedit, &QLineEdit::textChanged, this, &LinebreaksViewerWindow::onParameterChanged);
  connect(m_rightskip_lineedit, &QLineEdit::textChanged, this, &LinebreaksViewerWindow::onParameterChanged);
  connect(m_baselineskip_lineedit, &QLineEdit::textChanged, this, &LinebreaksViewerWindow::onParameterChanged);
  connect(m_lineskip_lineedit, &QLineEdit::textChanged, this, &LinebreaksViewerWindow::onParameterChanged);
  connect(m_lineskiplimit_lineedit, &QLineEdit::textChanged, this, &LinebreaksViewerWindow::onParameterChanged);
  connect(m_hangindent_input, &QLineEdit::textChanged, this, &LinebreaksViewerWindow::onParameterChanged);
  connect(m_hangafter_input, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &LinebreaksViewerWindow::onParameterChanged);
  connect(m_parshape_lineedit, &QLineEdit::textChanged, this, &LinebreaksViewerWindow::onParshapeChanged);
  connect(m_reset_button, &QPushButton::clicked, this, &LinebreaksViewerWindow::resetParameters);
  connect(m_linebreaks_spinbox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &LinebreaksViewerWindow::onSelectedBreakpointChanged);

  return w;
}

void LinebreaksViewerWindow::onTextChanged()
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

void LinebreaksViewerWindow::onDrawRatioChanged()
{
  m_renderwidget->show_ratios = m_draw_ratios->isChecked();
  update();
}

void LinebreaksViewerWindow::onParshapeChanged()
{
  m_parshape = parseParshape();

  if (m_parshape.empty() && !m_parshape_lineedit->text().isEmpty())
  {
    QPalette pal;
    pal.setColor(QPalette::Text, Qt::red);
    m_parshape_lineedit->setPalette(pal);
  }
  else
  {
    m_parshape_lineedit->setPalette(QPalette());
  }

  onParameterChanged();
}

void LinebreaksViewerWindow::onParameterChanged()
{
  write(m_leftskip, m_leftskip_lineedit);
  write(m_rightskip, m_rightskip_lineedit);
  write(m_baselineskip, m_baselineskip_lineedit);
  write(m_lineskip, m_lineskip_lineedit);

  write(m_lineskiplimit, m_lineskiplimit_lineedit);
  write(m_hangindent, m_hangindent_input);

  onTextChanged();
}

void LinebreaksViewerWindow::resetParameters()
{
  m_tolerance_spinbox->setValue(800);
  m_adjdemerits_spinbox->setValue(10000);
  m_linepenalty_spinbox->setValue(10);
  m_parshape_lineedit->clear();

  m_leftskip_lineedit->setText("0pt");
  m_rightskip_lineedit->setText("0pt");
  m_baselineskip_lineedit->setText("12pt");
  m_lineskip_lineedit->setText("1pt");
  m_lineskiplimit_lineedit->setText("0pt");
  m_hangindent_input->setText("0pt");
  m_hangafter_input->setValue(1);
}

void LinebreaksViewerWindow::onSelectedBreakpointChanged()
{
  if (m_breakpoints.empty())
    return;

  auto br = m_breakpoints.at(m_linebreaks_spinbox->value());

  tex::Paragraph paragraph;
  setup(paragraph);

  tex::FontMetrics metrics{ tex::Font(0), m_engine->metrics() };

  auto list = paragraph.create(m_list, paragraph.computeBreakpoints(br));

  auto box = tex::vbox(std::move(list));

  m_renderwidget->setBox(box);

  m_demerits_label->setText("Demerits: " + QString::number(br->demerits));
}

static double duration_msec(std::chrono::duration<double> diff)
{
  return diff.count() * 1000;
}

void LinebreaksViewerWindow::setup(tex::Paragraph& linebreaker)
{
  linebreaker.parshape = m_parshape;
  linebreaker.tolerance = m_tolerance_spinbox->value();
  linebreaker.adjdemerits = m_adjdemerits_spinbox->value();
  linebreaker.linepenalty = m_linepenalty_spinbox->value();

  linebreaker.leftskip = m_leftskip;
  linebreaker.rightskip = m_rightskip;
  linebreaker.baselineskip = m_baselineskip;
  linebreaker.lineskip = m_lineskip;
  linebreaker.lineskiplimit = m_lineskiplimit;
  linebreaker.hangindent = m_hangindent;
  linebreaker.hangafter = m_hangafter_input->value();
}

void LinebreaksViewerWindow::write(std::shared_ptr<tex::Glue>& g, QLineEdit* lineedit)
{
  try
  {
    tex::parsing::GlueParser parser{ m_unitsystem };

    const std::string str = lineedit->text().toStdString();

    for (size_t i(0); i < str.size(); ++i)
    {
      char c = str.at(i);
      parser.write(c);
    }

    g = parser.finish();

    lineedit->setPalette(QPalette());

    statusBar()->clearMessage();
  }
  catch (const std::runtime_error & ex)
  {
    statusBar()->showMessage(ex.what());

    QPalette pal;
    pal.setColor(QPalette::Text, Qt::red);
    lineedit->setPalette(pal);
  }
}

void LinebreaksViewerWindow::write(float& space, QLineEdit* lineedit)
{
  try
  {
    tex::parsing::DimenParser parser{ };

    const std::string str = lineedit->text().toStdString();

    for (size_t i(0); i < str.size(); ++i)
    {
      char c = str.at(i);
      parser.write(c);
    }

    tex::Dimen d = parser.finish();

    if (!d.isFinite())
      throw std::runtime_error{ "expected finite dimen" };

    space = d(m_unitsystem);
    lineedit->setPalette(QPalette());
  }
  catch (const std::runtime_error & ex)
  {
    statusBar()->showMessage(ex.what());

    QPalette pal;
    pal.setColor(QPalette::Text, Qt::red);
    lineedit->setPalette(pal);
  }
}

tex::Parshape LinebreaksViewerWindow::parseParshape() const
{
  QStringList numbers = m_parshape_lineedit->text().split(' ', QString::SkipEmptyParts);

  if (numbers.isEmpty() || numbers.size() % 2 != 0)
    return {};

  tex::Parshape result;

  for (int i(0); i < numbers.size(); i +=2)
  {
    result.emplace_back();
    result.back().indent = numbers.at(i).toFloat();
    result.back().length = std::max({ 10.f, numbers.at(i + 1).toFloat() });
  }

  return result;
}

void LinebreaksViewerWindow::processText()
{
  std::string text = m_textedit->toPlainText().toStdString();

  if (text.empty())
    return;

  tex::HListBuilder builder{ m_engine };
  static_cast<QtFontMetricsProdiver*>(m_engine->metrics().get())->frenchspacing = m_frenchspacing_input->isChecked();

  m_list.clear();

  for (auto it = text.cbegin(); it != text.cend();)
  {
    tex::Character c = tex::read_utf8_char(it);

    if (c == ' ' || c == '\n')
    {
      builder.push_back_interword_glue();
    }
    else
    {
      builder.push_back(c);
    }
  }

  m_list = std::move(builder.result);

  tex::Paragraph paragraph;
  setup(paragraph);

  paragraph.prepare(m_list);

  auto start = std::chrono::high_resolution_clock::now();

  auto breakpoint_list = paragraph.computeFeasibleBreakpoints(m_list);

  auto end = std::chrono::high_resolution_clock::now();

  m_breakpoints = std::vector<std::shared_ptr<tex::Paragraph::Breakpoint>>(breakpoint_list.begin(), breakpoint_list.end());

  std::sort(m_breakpoints.begin(), m_breakpoints.end(), [](const std::shared_ptr<tex::Paragraph::Breakpoint>& lhs, const std::shared_ptr<tex::Paragraph::Breakpoint>& rhs) {
    return lhs->demerits < rhs->demerits;
    });

  m_linebreaks_spinbox->setRange(0, m_breakpoints.size() - 1);
  m_linebreaks_spinbox->setValue(0);

  m_nblinebreaks_label->setText(QString::number(m_breakpoints.size()) + " alternatives");

  onSelectedBreakpointChanged();

  QString report = 
    "Linebreaking: " + QString::number(duration_msec(end - start));
  m_report_widget->setText(report);
}

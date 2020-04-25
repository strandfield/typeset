// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "qt-typeset-engine.h"

#include "tex/mathchars.h"

#include <QFontMetricsF>

#include <cmath>

QtFontMetrics::QtFontMetrics()
  : QFontMetricsF{ QFont{} }
{

}

QtFontMetricsProdiver::QtFontMetricsProdiver(FontTable & fonts)
  : m_fonts(fonts)
{
 
}

tex::BoxMetrics QtFontMetricsProdiver::metrics(tex::Character c, tex::Font font)
{
  const QFontMetricsF& m = info(font);
  QChar qc = c;
  QRectF rect = m.boundingRect(qc);

  tex::BoxMetrics ret;
  //ret.width = rect.width();
  ret.width = m.width(qc);
  ret.height = -rect.top();
  ret.depth = rect.bottom();
  return ret;
}

tex::BoxMetrics QtFontMetricsProdiver::metrics(const std::shared_ptr<tex::Symbol> & symbol, tex::Font font)
{
  if (symbol->isMathSymbol())
  {
    const QFontMetricsF& m = info(font);
    QChar c = static_cast<tex::MathSymbol*>(symbol.get())->character();
    QRectF rect = m.boundingRect(c);

    tex::BoxMetrics ret;
    //ret.width = rect.width();
    ret.width = m.width(c);
    ret.height = -rect.top();
    ret.depth = rect.bottom();
    return ret;
  }
  else
  {
    throw std::runtime_error{ "QtFontMetricsProdiver::metrics() - supports only mathsymbol" };
  }
}

float QtFontMetricsProdiver::italicCorrection(const std::shared_ptr<tex::Symbol> & symbol, tex::Font font)
{
  return 0;
}

int QtFontMetricsProdiver::sfcode(tex::Character c)
{
  if (frenchspacing)
    return 1000;

  if (c == '.' || c == '?' || c == '!')
    return 3000;
  else if (c == ':')
    return 2000;
  else if (c == ';')
    return 1500;
  else if (c == ',')
    return 1250;

  return 1000;
}

const tex::FontDimen& QtFontMetricsProdiver::fontdimen(tex::Font f)
{
  return m_fonts[f.id()].fontdimen;
}

const QFontMetricsF & QtFontMetricsProdiver::info(tex::Font f) const
{
  return m_fonts[f.id()].metrics;
}

CharBox::CharBox(tex::Character c, tex::Font f, tex::BoxMetrics box, const QFont& qf)
  : tex::CharacterBox(c, f, box),
    qfont(qf),
    oribox(box)
{
  text = QString(QChar(c));
}

void CharBox::deform(const tex::BoxMetrics& boxmetrics)
{
  reset(boxmetrics.height, boxmetrics.depth, boxmetrics.width);
  deformed = true;
}

std::shared_ptr<tex::FontMetricsProvider> TypesetEngine::metrics() const
{
  return mMetrics;
}

TypesetEngine::TypesetEngine(float mag)
  : m_mag(mag)
{
  reset(mag);
}

void TypesetEngine::reset(float mag)
{
  m_mag = mag;

  initFont(0, "textfont0", "Times New Roman", 10, false, tex::tfm::cmr10());
  initFont(1, "scriptfont0", "Times New Roman", 7, false, tex::tfm::cmr7());
  initFont(2, "scriptscriptfont0", "Times New Roman", 5, false, tex::tfm::cmr5());

  initFont(3, "textfont1", "Times New Roman", 10, true, tex::tfm::cmmi10());
  initFont(4, "scriptfont1", "Times New Roman", 7, true, tex::tfm::cmmi7());
  initFont(5, "scriptscriptfont1", "Times New Roman", 5, true, tex::tfm::cmmi5());

  initFont(6, "textfont2", "Times New Roman", 10, false, tex::tfm::cmsy10());
  initFont(7, "scriptfont2", "Times New Roman", 7, false, tex::tfm::cmsy7());
  initFont(8, "scriptscriptfont2", "Times New Roman", 5, false, tex::tfm::cmsy5());

  initFont(9, "textfont3", "Times New Roman", 10, false, tex::tfm::cmex10());
  initFont(10, "scriptfont3", "Times New Roman", 7, false, tex::tfm::cmex10());
  initFont(11, "scriptscriptfont3", "Times New Roman", 5, false, tex::tfm::cmex10());

  const int class_num = 11; // this class num is not valid, but equals to math::Atom::Rad
  const int fam = 3;
  mRadicalSign = std::make_shared<tex::MathSymbol>(tex::mathchars::SQRT, class_num, fam);

  mMetrics = std::make_shared<QtFontMetricsProdiver>(m_fonts);
}

const FontTable& TypesetEngine::fonts() const
{
  return m_fonts;
}

std::array<tex::MathFont, 16> TypesetEngine::mathfonts() const
{
  std::array<tex::MathFont, 16> result;

  for (size_t i(0); i < 16; ++i)
  {
    result[i].textfont = tex::Font(3 * i);
    result[i].scriptfont = tex::Font(3 * i + 1);
    result[i].scriptscriptfont = tex::Font(3 * i + 2);
  }

  return result;
}

std::shared_ptr<tex::Box> TypesetEngine::typeset(tex::Character c, tex::Font font)
{
  tex::BoxMetrics box = metrics()->metrics(c, font);
  return std::make_shared<CharBox>(c, font, box, this->font(font));
}

std::shared_ptr<tex::Box> TypesetEngine::typeset(const std::string& text, tex::Font font)
{
  // @TODO: handle this case
  throw std::runtime_error{ "TypesetEngine::typeset() : text typesetting not implemented" };
}

std::shared_ptr<tex::Box> TypesetEngine::typeset(const std::shared_ptr<tex::Symbol> & symbol, tex::Font font)
{
  if (symbol->isMathSymbol())
  {
    tex::Character c = static_cast<tex::MathSymbol*>(symbol.get())->character();
    tex::BoxMetrics box = metrics()->metrics(symbol, font);
    return std::make_shared<CharBox>(c, font, box, this->font(font));
  }
  else
  {
    Q_ASSERT(symbol->is<tex::TextSymbol>());
    return typeset(symbol->as<tex::TextSymbol>().text(), font);
  }
}

std::shared_ptr<tex::Box> TypesetEngine::typesetRadicalSign(float minTotalHeight)
{
  tex::Font font = tex::Font(mRadicalSign->family() * 3);
  auto metrics = mMetrics->metrics(mRadicalSign, font);
  auto ret = std::make_shared<CharBox>(mRadicalSign->character(), font, metrics, m_fonts[font.id()].font);
  const float ratio = minTotalHeight / (metrics.height + metrics.depth);

  if (ratio > 1.f)
  {
    metrics.height *= ratio;
    metrics.depth *= ratio;
    ret->deform(metrics);
    ret->rawfont = m_fonts[mRadicalSign->family() * 3].rawfont;
  }

  return ret;
}

std::shared_ptr<tex::Box> TypesetEngine::typesetDelimiter(const std::shared_ptr<tex::Symbol> & symbol, float minTotalHeight)
{
  auto mathsymbol = std::static_pointer_cast<tex::MathSymbol>(symbol);

  tex::Font font = tex::Font(mathsymbol->family() * 3);
  auto metrics = mMetrics->metrics(mathsymbol, font);
  auto ret = std::make_shared<CharBox>(mathsymbol->character(), font, metrics, m_fonts[font.id()].font);
  const float ratio = minTotalHeight / (metrics.height + metrics.depth);

  if (ratio > 1.f)
  {
    metrics.height *= ratio;
    metrics.depth *= ratio;
    ret->deform(metrics);
    ret->rawfont = m_fonts[mathsymbol->family() * 3].rawfont;
  }

  return ret;
}

std::shared_ptr<tex::Box> TypesetEngine::typesetLargeOp(const std::shared_ptr<tex::Symbol> & symbol)
{
  return typeset(symbol, tex::Font::MathRoman);
}

QFont & TypesetEngine::font(tex::Font f)
{
  return m_fonts[f.id()].font;
}

void TypesetEngine::initFont(int id, const QString& displayname, const QString & fontname, int size, bool italic, tex::TFM tfm)
{
  QFont qfont{ fontname };
  qfont.setItalic(italic);
  qfont.setPointSize(size * m_mag);
  m_fonts[id].name = displayname;
  m_fonts[id].font = qfont;
  m_fonts[id].rawfont = QRawFont::fromFont(qfont);
  m_fonts[id].metrics = QFontMetrics{ qfont };
  
  tfm.design_size = m_fonts[id].metrics.height();
  tfm = tex::tfm::to_absolute(tfm);
  m_fonts[id].fontdimen = tfm.fontdimen;
}

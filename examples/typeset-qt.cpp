// Copyright (C) 2019-2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "typeset-qt.h"

#include "stringbox.h"

#include <QFontMetricsF>

#include <cmath>

QChar QCharSymbol::get(const tex::Symbol & s)
{
  return static_cast<const QCharSymbol &>(s).character();
}

QtFontMetrics::QtFontMetrics()
  : QFontMetricsF{ QFont{} }
{

}

QtFontMetricsProdiver::QtFontMetricsProdiver(const FontTable & fonts)
  : m_fonts(fonts)
{
 
}

tex::BoxMetrics QtFontMetricsProdiver::metrics(tex::Character c, tex::Font font)
{
  throw std::runtime_error{ "Not implemented" };
}

tex::BoxMetrics QtFontMetricsProdiver::metrics(const std::shared_ptr<tex::Symbol> & symbol, tex::Font font)
{
  const QFontMetricsF & m = info(font);
  QChar c = QCharSymbol::get(*symbol);
  QRectF rect = m.boundingRect(c);

  tex::BoxMetrics ret;
  ret.width = rect.width();
  ret.height = -rect.top();
  ret.depth = rect.bottom();
  return ret;
}

float QtFontMetricsProdiver::italicCorrection(const std::shared_ptr<tex::Symbol> & symbol, tex::Font font)
{
  return 0;
}

const tex::FontDimen& QtFontMetricsProdiver::fontdimen(tex::Font f)
{
  return m_fonts[f.id()].fontdimen;
}

const QFontMetricsF & QtFontMetricsProdiver::info(tex::Font f) const
{
  return m_fonts[f.id()].metrics;
}

std::shared_ptr<tex::FontMetricsProvider> QtTypesetEngine::metrics() const
{
  return mMetrics;
}

QtTypesetEngine::QtTypesetEngine()
{
  // textfont0
  initFont(0, "Times New Roman", 10, false, tex::tfm::cmr10());
  initFont(1, "Times New Roman", 7, false, tex::tfm::cmr7());
  initFont(2, "Times New Roman", 5, false, tex::tfm::cmr5());
  // textfont1
  initFont(3, "Times New Roman", 10, true, tex::tfm::cmmi10());
  initFont(4, "Times New Roman", 7, true, tex::tfm::cmmi7());
  initFont(5, "Times New Roman", 5, true, tex::tfm::cmmi5());
  // textfont2
  initFont(6, "Times New Roman", 10, false, tex::tfm::cmsy10());
  initFont(7, "Times New Roman", 7, false, tex::tfm::cmsy7());
  initFont(8, "Times New Roman", 5, false, tex::tfm::cmsy5());
  // textfont3
  initFont(9, "Times New Roman", 10, false, tex::tfm::cmex10());
  initFont(10, "Times New Roman", 7, false, tex::tfm::cmex10());
  initFont(11, "Times New Roman", 5, false, tex::tfm::cmex10());

  QChar c{ 0x221A };
  mRadicalSign = std::make_shared<QCharSymbol>(c);

  mMetrics = std::make_shared<QtFontMetricsProdiver>(m_fonts);
}

const FontTable& QtTypesetEngine::fonts() const
{
  return m_fonts;
}

std::shared_ptr<tex::Box> QtTypesetEngine::typeset(tex::Character c, tex::Font font)
{
  throw std::runtime_error{ "Not implemented" };
}

std::shared_ptr<tex::Box> QtTypesetEngine::typeset(const std::string& text, tex::Font font)
{
  return stringbox(QString::fromUtf8(text.data(), text.size()), this->font(font));
}

std::shared_ptr<tex::Box> QtTypesetEngine::typeset(const std::shared_ptr<tex::Symbol> & symbol, tex::Font font)
{
  if (symbol->isMathSymbol())
  {
    QChar c = static_cast<tex::MathSymbol*>(symbol.get())->character();
    return std::make_shared<StringBox>(c, this->font(font));
  }
  else if (symbol->is<QCharSymbol>())
  {
    QChar c = QCharSymbol::get(*symbol);
    return std::make_shared<StringBox>(c, this->font(font));
  }
  else
  {
    Q_ASSERT(symbol->is<tex::TextSymbol>());
    return typeset(symbol->as<tex::TextSymbol>().text(), font);
  }
}

std::shared_ptr<tex::Box> QtTypesetEngine::typesetRadicalSign(float minTotalHeight)
{
  QChar c = QCharSymbol::get(*mRadicalSign);
  auto metrics = mMetrics->metrics(mRadicalSign, tex::Font::MathRoman);
  const float ratio = minTotalHeight / (metrics.height + metrics.depth);
  metrics.height *= ratio;
  metrics.depth *= ratio;
  return std::make_shared<StringBox>(c, this->font(tex::Font::MathRoman), metrics);
}

std::shared_ptr<tex::Box> QtTypesetEngine::typesetDelimiter(const std::shared_ptr<tex::Symbol> & symbol, float minTotalHeight)
{
  QChar c = QCharSymbol::get(*symbol);
  auto metrics = mMetrics->metrics(symbol, tex::Font::MathRoman);
  const float ratio = minTotalHeight / (metrics.height + metrics.depth);
  metrics.height *= ratio;
  metrics.depth *= ratio;
  return std::make_shared<StringBox>(QString(c), this->font(tex::Font::MathRoman), metrics);
}

std::shared_ptr<tex::Box> QtTypesetEngine::typesetLargeOp(const std::shared_ptr<tex::Symbol> & symbol)
{
  return typeset(symbol, tex::Font::MathRoman);
}

QFont & QtTypesetEngine::font(tex::Font f)
{
  return m_fonts[f.id()].font;
}

void QtTypesetEngine::initFont(int id, const QString & name, int size, bool italic, tex::TFM tfm)
{
  QFont qfont{ name };
  qfont.setItalic(italic);
  qfont.setPointSize(size);
  m_fonts[id].font = qfont;
  m_fonts[id].metrics = QFontMetrics{ qfont };
  
  tfm.design_size = m_fonts[id].metrics.height();
  tfm = tex::tfm::to_absolute(tfm);
  m_fonts[id].fontdimen = tfm.fontdimen;
}

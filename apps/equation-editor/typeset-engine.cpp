// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "typeset-engine.h"

#include "tex/mathchars.h"

#include <QFontMetricsF>

#include <cmath>

QtFontMetrics::QtFontMetrics()
  : QFontMetricsF{ QFont{} }
{

}

QtFontMetricsProdiver::QtFontMetricsProdiver(const FontTable & fonts)
  : m_fonts(fonts)
{
 
}

tex::BoxMetrics QtFontMetricsProdiver::metrics(const std::shared_ptr<tex::Symbol> & symbol, tex::Font font)
{
  if (symbol->isMathSymbol())
  {
    const QFontMetricsF& m = info(font);
    QChar c = static_cast<tex::MathSymbol*>(symbol.get())->character();
    QRectF rect = m.boundingRect(c);

    tex::BoxMetrics ret;
    ret.width = rect.width();
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
    qfont(qf)
{
  text = QString(QChar(c));
}

void CharBox::deform(const tex::BoxMetrics& boxmetrics)
{
  reset(boxmetrics.height, boxmetrics.depth, boxmetrics.width);
  deformed = true;
}

std::shared_ptr<tex::FontMetricsProdiver> TypesetEngine::metrics() const
{
  return mMetrics;
}

TypesetEngine::TypesetEngine()
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
    result[i].textfont = tex::Font(3 * i + 1);
    result[i].scriptfont = tex::Font(3 * i + 2);
    result[i].scriptscriptfont = tex::Font(3 * i + 3);
  }

  return result;
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
  const float ratio = minTotalHeight / (metrics.height + metrics.depth);
  metrics.height *= ratio;
  metrics.depth *= ratio;
  return std::make_shared<CharBox>(mRadicalSign->character(), font, metrics, m_fonts[font.id()].font);
}

std::shared_ptr<tex::Box> TypesetEngine::typesetDelimiter(const std::shared_ptr<tex::Symbol> & symbol, float minTotalHeight)
{
  tex::Font font = tex::Font(mRadicalSign->family() * 3);
  auto metrics = mMetrics->metrics(mRadicalSign, font);
  const float ratio = minTotalHeight / (metrics.height + metrics.depth);
  metrics.height *= ratio;
  metrics.depth *= ratio;
  auto mathsym = std::static_pointer_cast<tex::MathSymbol>(symbol);
  return std::make_shared<CharBox>(mathsym->character(), font, metrics, m_fonts[font.id()].font);
}

std::shared_ptr<tex::Box> TypesetEngine::typesetLargeOp(const std::shared_ptr<tex::Symbol> & symbol)
{
  return typeset(symbol, tex::Font::MathRoman);
}

QFont & TypesetEngine::font(tex::Font f)
{
  return m_fonts[f.id()].font;
}

void TypesetEngine::initFont(int id, const QString & name, int size, bool italic, tex::TFM tfm)
{
  QFont qfont{ name };
  qfont.setItalic(italic);
  qfont.setPointSize(size * 2);
  m_fonts[id].font = qfont;
  m_fonts[id].metrics = QFontMetrics{ qfont };
  
  tfm.design_size = m_fonts[id].metrics.height();
  tfm = tex::tfm::to_absolute(tfm);
  m_fonts[id].fontdimen = tfm.fontdimen;
}

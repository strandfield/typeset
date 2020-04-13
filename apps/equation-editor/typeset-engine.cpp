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

QtFontMetricsProdiver::QtFontMetricsProdiver(FontTable & fonts)
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

float QtFontMetricsProdiver::slantPerPt(tex::Font font)
{
  m_fonts[font.id()].fontdimenusage.slant_per_pt = true;
  return m_fonts[font.id()].fontdimen.slant_per_pt;
}

float QtFontMetricsProdiver::interwordSpace(tex::Font font)
{
  m_fonts[font.id()].fontdimenusage.interword_space = true;
  return m_fonts[font.id()].fontdimen.interword_space;
}

float QtFontMetricsProdiver::interwordStretch(tex::Font font)
{
  m_fonts[font.id()].fontdimenusage.interword_stretch = true;
  return m_fonts[font.id()].fontdimen.interword_stretch;
}

float QtFontMetricsProdiver::interwordShrink(tex::Font font)
{
  m_fonts[font.id()].fontdimenusage.interword_shrink = true;
  return m_fonts[font.id()].fontdimen.interword_shrink;
}

float QtFontMetricsProdiver::extraSpace(tex::Font font)
{
  m_fonts[font.id()].fontdimenusage.extra_space = true;
  return m_fonts[font.id()].fontdimen.extra_space;
}


float QtFontMetricsProdiver::xHeight(tex::Font font)
{
  m_fonts[font.id()].fontdimenusage.x_height = true;
  return m_fonts[font.id()].fontdimen.x_height;
}

float QtFontMetricsProdiver::quad(tex::Font font)
{
  m_fonts[font.id()].fontdimenusage.quad = true;
  return m_fonts[font.id()].fontdimen.quad;
}

float QtFontMetricsProdiver::num1(tex::Font font)
{
  m_fonts[font.id()].fontdimenusage.num1 = true;
  return m_fonts[font.id()].fontdimen.num1;
}

float QtFontMetricsProdiver::num2(tex::Font font)
{
  m_fonts[font.id()].fontdimenusage.num2 = true;
  return m_fonts[font.id()].fontdimen.num2;
}

float QtFontMetricsProdiver::num3(tex::Font font)
{
  m_fonts[font.id()].fontdimenusage.num3 = true;
  return m_fonts[font.id()].fontdimen.num3;
}

float QtFontMetricsProdiver::denom1(tex::Font font)
{
  m_fonts[font.id()].fontdimenusage.denom1 = true;
  return m_fonts[font.id()].fontdimen.denom1;
}

float QtFontMetricsProdiver::denom2(tex::Font font)
{
  m_fonts[font.id()].fontdimenusage.denom2 = true;
  return m_fonts[font.id()].fontdimen.denom2;
}

float QtFontMetricsProdiver::sup1(tex::Font font)
{
  m_fonts[font.id()].fontdimenusage.sup1 = true;
  return m_fonts[font.id()].fontdimen.sup1;
}

float QtFontMetricsProdiver::sup2(tex::Font font)
{
  m_fonts[font.id()].fontdimenusage.sup2 = true;
  return m_fonts[font.id()].fontdimen.sup2;
}

float QtFontMetricsProdiver::sup3(tex::Font font)
{
  m_fonts[font.id()].fontdimenusage.sup3 = true;
  return m_fonts[font.id()].fontdimen.sup3;
}

float QtFontMetricsProdiver::sub1(tex::Font font)
{
  m_fonts[font.id()].fontdimenusage.sub1 = true;
  return m_fonts[font.id()].fontdimen.sub1;
}

float QtFontMetricsProdiver::sub2(tex::Font font)
{
  m_fonts[font.id()].fontdimenusage.sub2 = true;
  return m_fonts[font.id()].fontdimen.sub2;
}

float QtFontMetricsProdiver::supDrop(tex::Font font)
{
  m_fonts[font.id()].fontdimenusage.sup_drop = true;
  return m_fonts[font.id()].fontdimen.sup_drop;
}

float QtFontMetricsProdiver::subDrop(tex::Font font)
{
  m_fonts[font.id()].fontdimenusage.sub_drop = true;
  return m_fonts[font.id()].fontdimen.sub_drop;
}

float QtFontMetricsProdiver::delim1(tex::Font font)
{
  m_fonts[font.id()].fontdimenusage.delim1 = true;
  return m_fonts[font.id()].fontdimen.delim1;
}

float QtFontMetricsProdiver::delim2(tex::Font font)
{
  m_fonts[font.id()].fontdimenusage.delim2 = true;
  return m_fonts[font.id()].fontdimen.delim2;
}

float QtFontMetricsProdiver::axisHeight(tex::Font font)
{
  m_fonts[font.id()].fontdimenusage.axis_height = true;
  return m_fonts[font.id()].fontdimen.axis_height;
}

float QtFontMetricsProdiver::defaultRuleThickness(tex::Font font)
{
  m_fonts[font.id()].fontdimenusage.default_rule_thickness = true;
  return m_fonts[font.id()].fontdimen.default_rule_thickness;
}

float QtFontMetricsProdiver::bigOpSpacing1(tex::Font font)
{
  m_fonts[font.id()].fontdimenusage.big_op_spacing1 = true;
  return m_fonts[font.id()].fontdimen.big_op_spacing1;
}

float QtFontMetricsProdiver::bigOpSpacing2(tex::Font font)
{
  m_fonts[font.id()].fontdimenusage.big_op_spacing2 = true;
  return m_fonts[font.id()].fontdimen.big_op_spacing2;
}

float QtFontMetricsProdiver::bigOpSpacing3(tex::Font font)
{
  m_fonts[font.id()].fontdimenusage.big_op_spacing3 = true;
  return m_fonts[font.id()].fontdimen.big_op_spacing3;
}

float QtFontMetricsProdiver::bigOpSpacing4(tex::Font font)
{
  m_fonts[font.id()].fontdimenusage.big_op_spacing4 = true;
  return m_fonts[font.id()].fontdimen.big_op_spacing4;
}

float QtFontMetricsProdiver::bigOpSpacing5(tex::Font font)
{
  m_fonts[font.id()].fontdimenusage.big_op_spacing5 = true;
  return m_fonts[font.id()].fontdimen.big_op_spacing5;
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

std::shared_ptr<tex::FontMetricsProvider> TypesetEngine::metrics() const
{
  return mMetrics;
}

TypesetEngine::TypesetEngine()
{
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

void TypesetEngine::reset()
{
  for (auto& f : m_fonts)
  {
    f.fontdimenusage = FontDimenUsage();
  }
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

void TypesetEngine::initFont(int id, const QString& displayname, const QString & fontname, int size, bool italic, tex::TFM tfm)
{
  QFont qfont{ fontname };
  qfont.setItalic(italic);
  qfont.setPointSize(size * 2);
  m_fonts[id].name = displayname;
  m_fonts[id].font = qfont;
  m_fonts[id].metrics = QFontMetrics{ qfont };
  
  tfm.design_size = m_fonts[id].metrics.height();
  tfm = tex::tfm::to_absolute(tfm);
  m_fonts[id].fontdimen = tfm.fontdimen;
}

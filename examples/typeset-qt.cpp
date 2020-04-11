// Copyright (C) 2019 Vincent Chambrin
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
//
//float QCharBox::height() const
//{
//  return mMetrics.height;
//}
//
//float QCharBox::depth() const
//{
//  return mMetrics.depth;
//}
//
//float QCharBox::width() const
//{
//  return mMetrics.width;
//}

QtFontMetrics::QtFontMetrics()
  : QFontMetricsF{ QFont{} }
{

}

QtFontMetricsProdiver::QtFontMetricsProdiver(const QtFontTable & fonts)
{
  for (size_t i(0); i < fonts.size(); ++i)
  {
    mMetrics[i] = QFontMetricsF{ fonts[i] };
  }

  for (size_t i(0); i < fonts.size(); ++i)
  {
    const QFontMetricsF& m = info(tex::Font(i));
    auto& font_dimen = mFontDimen[i];

    font_dimen.slant_per_pt = 0.f;
    font_dimen.interword_space = m.width(' ');
    font_dimen.interword_stretch = 0.25f * font_dimen.interword_space;
    font_dimen.interword_shrink = 0.1f * font_dimen.interword_space;
    font_dimen.extra_space = font_dimen.interword_stretch;
    font_dimen.x_height = -m.boundingRect(QChar('x')).top();
    font_dimen.quad = m.width(QChar('m'));
    font_dimen.num1 = font_dimen.x_height * 0.5f;
    font_dimen.num2 = font_dimen.x_height / 3.f;
    font_dimen.num3 = font_dimen.x_height / 4.f;
    font_dimen.denom1 = font_dimen.x_height / 3.f;
    font_dimen.denom2 = font_dimen.x_height / 4.f;
    font_dimen.sup1 = font_dimen.quad / 4.f;
    font_dimen.sup2 = font_dimen.quad / 5.f;
    font_dimen.sup3 = font_dimen.quad / 6.f;
    font_dimen.sub1 = font_dimen.quad / 4.f;
    font_dimen.sub2 = font_dimen.quad / 5.f;
    font_dimen.sup_drop = font_dimen.x_height;
    font_dimen.sub_drop = font_dimen.x_height;
    font_dimen.delim1 = m.boundingRect(QChar('(')).height();
    font_dimen.delim2 = font_dimen.delim1 * 1.1f;
    font_dimen.axis_height = font_dimen.x_height / 2.f;
    font_dimen.default_rule_thickness = 2.f;
    font_dimen.big_op_spacing1 = font_dimen.x_height / 2.f;
    font_dimen.big_op_spacing2 = font_dimen.x_height / 2.f;
    font_dimen.big_op_spacing3 = font_dimen.x_height * 0.75f;
    font_dimen.big_op_spacing4 = font_dimen.x_height * 0.5f + m.height();
    font_dimen.big_op_spacing5 = font_dimen.x_height * 0.25f;
  }
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
  return mFontDimen[f.id()];
}




const QFontMetricsF & QtFontMetricsProdiver::info(tex::Font f) const
{
  return mMetrics[f.id()];
}

std::shared_ptr<tex::FontMetricsProdiver> QtTypesetEngine::metrics() const
{
  return mMetrics;
}

QtTypesetEngine::QtTypesetEngine()
{
  // textfont0
  initFont(0, "Times New Roman", 10);
  initFont(1, "Times New Roman", 7);
  initFont(2, "Times New Roman", 5);
  // textfont1
  initFont(3, "Times New Roman", 10, true);
  initFont(4, "Times New Roman", 7, true);
  initFont(5, "Times New Roman", 5, true);
  // textfont2
  initFont(6, "Times New Roman", 10);
  initFont(7, "Times New Roman", 7);
  initFont(8, "Times New Roman", 5);
  // textfont3
  initFont(9, "Times New Roman", 10);
  initFont(10, "Times New Roman", 7);
  initFont(11, "Times New Roman", 5);

  QChar c{ 0x221A };
  mRadicalSign = std::make_shared<QCharSymbol>(c);

  mMetrics = std::make_shared<QtFontMetricsProdiver>(mFonts);
}

const QtFontTable& QtTypesetEngine::fonts() const
{
  return mFonts;
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
  return mFonts[f.id()];
}

void QtTypesetEngine::initFont(int id, const QString & name, int size, bool italic)
{
  QFont qfont{ name };
  qfont.setItalic(italic);
  qfont.setPointSize(size);
  mFonts[id] = qfont;
}

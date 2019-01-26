// Copyright (C) 2018 Vincent Chambrin
// This file is part of the liblayout project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "typeset-qt.h"

#include "stringbox.h"

#include <QFontMetricsF>

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
  const size_t inner_size = fonts.front().size();

  for (size_t i(0); i < fonts.size(); ++i)
  {
    for (size_t j(0); j < inner_size; ++j)
    {
      mMetrics[i][j] = QFontMetricsF{ fonts[i][j] };
    }
  }
}

tex::BoxMetrics QtFontMetricsProdiver::metrics(const std::shared_ptr<tex::Symbol> & symbol, tex::Font font, tex::FontSize size)
{
  const QFontMetricsF & m = info(font, size);
  QChar c = QCharSymbol::get(*symbol);
  QRectF rect = m.boundingRect(c);

  tex::BoxMetrics ret;
  ret.width = rect.width();
  ret.height = -rect.top();
  ret.depth = rect.bottom();
  return ret;
}

float QtFontMetricsProdiver::italicCorrection(const std::shared_ptr<tex::Symbol> & symbol, tex::Font font, tex::FontSize size)
{
  return 0;
}

float QtFontMetricsProdiver::xHeight(tex::FontSize size)
{
  const float result = -info(tex::Font::MathRoman, size).boundingRect(QChar('x')).top();
  return result;
}

float QtFontMetricsProdiver::quad(tex::FontSize size)
{
  const float result = info(tex::Font::MathRoman, size).width(QChar('M'));
  return result;
}

float QtFontMetricsProdiver::num1(tex::FontSize size)
{
  float result = xHeight(size) / 2.f;
  return result;
}

float QtFontMetricsProdiver::num2(tex::FontSize size)
{
  float result = xHeight(size) / 3.f;
  return result;
}

float QtFontMetricsProdiver::num3(tex::FontSize size)
{
  float result = xHeight(size) / 4.f;
  return result;
}

float QtFontMetricsProdiver::denom1(tex::FontSize size)
{
  float result = xHeight(size) / 3.f;
  return result;
}

float QtFontMetricsProdiver::denom2(tex::FontSize size)
{
  float result = xHeight(size) / 4.f;
  return result;
}

float QtFontMetricsProdiver::sup1(tex::FontSize size)
{
  float result = quad(size) / 4.f;
  return result;
}

float QtFontMetricsProdiver::sup2(tex::FontSize size)
{
  float result = quad(size) / 5.f;
  return result;
}

float QtFontMetricsProdiver::sup3(tex::FontSize size)
{
  float result = quad(size) / 6.f;
  return result;
}

float QtFontMetricsProdiver::sub1(tex::FontSize size)
{
  float result = quad(size) / 4.f;
  return result;
}

float QtFontMetricsProdiver::sub2(tex::FontSize size)
{
  float result = quad(size) / 5.f;
  return result;
}

float QtFontMetricsProdiver::supDrop(tex::FontSize size)
{
  float result = xHeight(size);
  return result;
}

float QtFontMetricsProdiver::subDrop(tex::FontSize size)
{
  float result = xHeight(size);
  return result;
}

float QtFontMetricsProdiver::delim1(tex::FontSize size)
{
  const auto rect = info(tex::Font::MathRoman, size).boundingRect(QChar('('));
  return rect.height();
}

float QtFontMetricsProdiver::delim2(tex::FontSize size)
{
  const auto rect = info(tex::Font::MathRoman, size).boundingRect(QChar('('));
  float result = rect.height() * 1.1f;
  return result;
}

float QtFontMetricsProdiver::axisHeight(tex::FontSize size)
{
  float result = xHeight(size) / 2.f;
  return result;
}

float QtFontMetricsProdiver::defaultRuleThickness()
{
  return 2.f;
}

float QtFontMetricsProdiver::bigOpSpacing1()
{
  float result = xHeight(tex::FontSize::Normal) / 2.f;
  return result;
}

float QtFontMetricsProdiver::bigOpSpacing2()
{
  float result = xHeight(tex::FontSize::Normal) / 2.f;
  return result;
}

float QtFontMetricsProdiver::bigOpSpacing3()
{
  float result = xHeight(tex::FontSize::Normal) * 0.75f;
  return result;
}

float QtFontMetricsProdiver::bigOpSpacing4()
{
  float result = xHeight(tex::FontSize::Normal) * 0.5f + info(tex::Font::MathRoman, tex::FontSize::Normal).height();
  return result;
}

float QtFontMetricsProdiver::bigOpSpacing5()
{
  float result = xHeight(tex::FontSize::Normal) * 0.25f;
  return result;
}

const QFontMetricsF & QtFontMetricsProdiver::info(tex::Font f, tex::FontSize s) const
{
  return mMetrics[f.id()][s.value() - 1];
}

std::shared_ptr<tex::FontMetricsProdiver> QtTypesetEngine::metrics() const
{
  return mMetrics;
}

QtTypesetEngine::QtTypesetEngine()
{
  initFont(tex::Font::Default, "Times New Roman", 24);
  initFont(tex::Font::Text, "Times New Roman", 24);
  initFont(tex::Font::MathRoman, "Times New Roman", 24);
  initFont(tex::Font::MathItalic, "Times New Roman", 24, true);

  QChar c{ 0x221A };
  mRadicalSign = std::make_shared<QCharSymbol>(c);

  mMetrics = std::make_shared<QtFontMetricsProdiver>(mFonts);
}

std::shared_ptr<tex::Box> QtTypesetEngine::typeset(const std::shared_ptr<tex::Symbol> & symbol, tex::Font font, tex::FontSize size)
{
  QChar c = QCharSymbol::get(*symbol);
  return std::make_shared<StringBox>(c, this->font(font, size));
}

std::shared_ptr<tex::Box> QtTypesetEngine::typesetRadicalSign(float minTotalHeight, tex::FontSize size)
{
  QChar c = QCharSymbol::get(*mRadicalSign);
  auto metrics = mMetrics->metrics(mRadicalSign, tex::Font::MathRoman, size);
  const float ratio = minTotalHeight / (metrics.height + metrics.depth);
  metrics.height *= ratio;
  metrics.depth *= ratio;
  return std::make_shared<StringBox>(c, this->font(tex::Font::MathRoman, size), metrics);
}

std::shared_ptr<tex::Box> QtTypesetEngine::typesetDelimiter(const std::shared_ptr<tex::Symbol> & symbol, float minTotalHeight, tex::FontSize size)
{
  QChar c = QCharSymbol::get(*symbol);
  auto metrics = mMetrics->metrics(symbol, tex::Font::MathRoman, size);
  const float ratio = minTotalHeight / (metrics.height + metrics.depth);
  metrics.height *= ratio;
  metrics.depth *= ratio;
  return std::make_shared<StringBox>(QString(c), this->font(tex::Font::MathRoman, size), metrics);
}

std::shared_ptr<tex::Box> QtTypesetEngine::typesetLargeOp(const std::shared_ptr<tex::Symbol> & symbol, tex::FontSize size)
{
  size = size.larger();
  return typeset(symbol, tex::Font::MathRoman, size);
}

QFont & QtTypesetEngine::font(tex::Font f, tex::FontSize s)
{
  return mFonts[f.id()][s.value() - 1];
}

static QFont font_with_size(QFont f, tex::FontSize size, int default_size)
{
  const float scale = 1.1f;
  const int step = size.value() - tex::FontSize::Normal.value();
  const float factor = std::pow(scale, step);
  f.setPixelSize(default_size * factor);
  return f;
}

static void init_font(QtFontTable::reference fonts, QFont f, tex::FontSize size, int default_size)
{
  fonts[size.value() - 1] = font_with_size(f, size, default_size);
}

void QtTypesetEngine::initFont(tex::Font f, const QString & name, int default_size, bool italic)
{
  QFont qfont{ name };
  qfont.setItalic(italic);
  auto &fonts = mFonts[f.id()];

  init_font(fonts, qfont, tex::FontSize::Normal, default_size);
  init_font(fonts, qfont, tex::FontSize::Small, default_size);
  init_font(fonts, qfont, tex::FontSize::Large, default_size);
  init_font(fonts, qfont, tex::FontSize::FootNoteSize, default_size);
  init_font(fonts, qfont, tex::FontSize::ScriptSize, default_size);
  init_font(fonts, qfont, tex::FontSize::Tiny, default_size);
  init_font(fonts, qfont, tex::FontSize::LARGE, default_size);
  init_font(fonts, qfont, tex::FontSize::Huge, default_size);
  init_font(fonts, qfont, tex::FontSize::HUGE, default_size);
}
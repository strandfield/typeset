// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/fontmetrics.h"

namespace tex
{

int FontMetricsProvider::sfcode(tex::Character c)
{
  return 1000;
}

float FontMetricsProvider::slantPerPt(Font font)
{
  return fontdimen(font).slant_per_pt;
}

float FontMetricsProvider::interwordSpace(Font font)
{
  return fontdimen(font).interword_space;
}

float FontMetricsProvider::interwordStretch(Font font)
{
  return fontdimen(font).interword_stretch;
}

float FontMetricsProvider::interwordShrink(Font font)
{
  return fontdimen(font).interword_shrink;
}

float FontMetricsProvider::extraSpace(Font font)
{
  return fontdimen(font).extra_space;
}


float FontMetricsProvider::xHeight(Font font)
{
  return fontdimen(font).x_height;
}

float FontMetricsProvider::quad(Font font)
{
  return fontdimen(font).quad;
}

float FontMetricsProvider::num1(Font font)
{
  return fontdimen(font).num1;
}

float FontMetricsProvider::num2(Font font)
{
  return fontdimen(font).num2;
}

float FontMetricsProvider::num3(Font font)
{
  return fontdimen(font).num3;
}

float FontMetricsProvider::denom1(Font font)
{
  return fontdimen(font).denom1;
}

float FontMetricsProvider::denom2(Font font)
{
  return fontdimen(font).denom2;
}

float FontMetricsProvider::sup1(Font font)
{
  return fontdimen(font).sup1;
}

float FontMetricsProvider::sup2(Font font)
{
  return fontdimen(font).sup2;
}

float FontMetricsProvider::sup3(Font font)
{
  return fontdimen(font).sup3;
}

float FontMetricsProvider::sub1(Font font)
{
  return fontdimen(font).sub1;
}

float FontMetricsProvider::sub2(Font font)
{
  return fontdimen(font).sub2;
}

float FontMetricsProvider::supDrop(Font font)
{
  return fontdimen(font).sup_drop;
}

float FontMetricsProvider::subDrop(Font font)
{
  return fontdimen(font).sub_drop;
}

float FontMetricsProvider::delim1(Font font)
{
  return fontdimen(font).delim1;
}

float FontMetricsProvider::delim2(Font font)
{
  return fontdimen(font).delim2;
}

float FontMetricsProvider::axisHeight(Font font)
{
  return fontdimen(font).axis_height;
}

float FontMetricsProvider::defaultRuleThickness(Font font)
{
  return fontdimen(font).default_rule_thickness;
}

float FontMetricsProvider::bigOpSpacing1(Font font)
{
  return fontdimen(font).big_op_spacing1;
}

float FontMetricsProvider::bigOpSpacing2(Font font)
{
  return fontdimen(font).big_op_spacing2;
}

float FontMetricsProvider::bigOpSpacing3(Font font)
{
  return fontdimen(font).big_op_spacing3;
}

float FontMetricsProvider::bigOpSpacing4(Font font)
{
  return fontdimen(font).big_op_spacing4;
}

float FontMetricsProvider::bigOpSpacing5(Font font)
{
  return fontdimen(font).big_op_spacing5;
}

FontMetrics::FontMetrics(Font font, std::shared_ptr<FontMetricsProvider> mp)
  : mFont(font)
  , mMetricsProvider(mp)
{

}

BoxMetrics FontMetrics::metrics(tex::Character c) const
{
  return metricsProvider()->metrics(c, font());
}

BoxMetrics FontMetrics::metrics(const std::shared_ptr<tex::Symbol> & symbol) const
{
  return metricsProvider()->metrics(symbol, font());
}

float FontMetrics::italicCorrection(const std::shared_ptr<tex::Symbol> & symbol) const
{
  return metricsProvider()->italicCorrection(symbol, font());
}

const FontDimen& FontMetrics::fontdimen() const
{
  return metricsProvider()->fontdimen(font());
}

float FontMetrics::slantPerPt() const
{
  return metricsProvider()->slantPerPt(mFont);
}

float FontMetrics::interwordSpace() const
{
  return metricsProvider()->interwordSpace(mFont);
}

float FontMetrics::interwordStretch() const
{
  return metricsProvider()->interwordStretch(mFont);
}

float FontMetrics::interwordShrink() const
{
  return metricsProvider()->interwordShrink(mFont);
}

float FontMetrics::extraSpace() const
{
  return metricsProvider()->extraSpace(mFont);
}

float FontMetrics::xHeight() const
{
  return metricsProvider()->xHeight(mFont);
}

float FontMetrics::quad() const
{
  return metricsProvider()->quad(mFont);
}

float FontMetrics::num1() const
{
  return metricsProvider()->num1(mFont);
}

float FontMetrics::num2() const
{
  return metricsProvider()->num2(mFont);
}

float FontMetrics::num3() const
{
  return metricsProvider()->num3(mFont);
}

float FontMetrics::denom1() const
{
  return metricsProvider()->denom1(mFont);
}

float FontMetrics::denom2() const
{
  return metricsProvider()->denom2(mFont);
}

float FontMetrics::sup1() const
{
  return metricsProvider()->sup1(mFont);
}

float FontMetrics::sup2() const
{
  return metricsProvider()->sup2(mFont);
}

float FontMetrics::sup3() const
{
  return metricsProvider()->sup3(mFont);
}

float FontMetrics::sub1() const
{
  return metricsProvider()->sub1(mFont);
}

float FontMetrics::sub2() const
{
  return metricsProvider()->sub2(mFont);
}

float FontMetrics::supDrop() const
{
  return metricsProvider()->supDrop(mFont);
}

float FontMetrics::subDrop() const
{
  return metricsProvider()->subDrop(mFont);
}

float FontMetrics::delim1() const
{
  return metricsProvider()->delim1(mFont);
}

float FontMetrics::delim2() const
{
  return metricsProvider()->delim2(mFont);
}

float FontMetrics::axisHeight() const
{
  return metricsProvider()->axisHeight(mFont);
}


float FontMetrics::defaultRuleThickness() const
{
  return metricsProvider()->defaultRuleThickness(mFont);
}

float FontMetrics::bigOpSpacing1() const
{
  return metricsProvider()->bigOpSpacing1(mFont);
}

float FontMetrics::bigOpSpacing2() const
{
  return metricsProvider()->bigOpSpacing2(mFont);
}

float FontMetrics::bigOpSpacing3() const
{
  return metricsProvider()->bigOpSpacing3(mFont);
}

float FontMetrics::bigOpSpacing4() const
{
  return metricsProvider()->bigOpSpacing4(mFont);
}

float FontMetrics::bigOpSpacing5() const
{
  return metricsProvider()->bigOpSpacing5(mFont);
}

} // namespace tex

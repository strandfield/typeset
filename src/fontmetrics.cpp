// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/fontmetrics.h"

namespace tex
{

FontMetrics::FontMetrics(Font font, FontSize size, std::shared_ptr<FontMetricsProdiver> mp)
  : mFont(font)
  , mFontSize(size)
  , mMetricsProvider(mp)
{

}

BoxMetrics FontMetrics::metrics(const std::shared_ptr<tex::Symbol> & symbol) const
{
  return metricsProvider()->metrics(symbol, font(), fontSize());
}

float FontMetrics::italicCorrection(const std::shared_ptr<tex::Symbol> & symbol) const
{
  return metricsProvider()->italicCorrection(symbol, font(), fontSize());
}

float FontMetrics::slantPerPt() const
{
  return metricsProvider()->slantPerPt(font());
}

float FontMetrics::interwordSpace() const
{
  return metricsProvider()->interwordSpace(font());
}

float FontMetrics::interwordStretch() const
{
  return metricsProvider()->interwordStretch(font());
}

float FontMetrics::interwordShrink() const
{
  return metricsProvider()->interwordShrink(font());
}

float FontMetrics::extraSpace() const
{
  return metricsProvider()->extraSpace(font());
}

float FontMetrics::xHeight() const
{
  return metricsProvider()->xHeight(fontSize());
}

float FontMetrics::quad() const
{
  return metricsProvider()->quad(fontSize());
}

float FontMetrics::num1() const
{
  return metricsProvider()->num1(fontSize());
}

float FontMetrics::num2() const
{
  return metricsProvider()->num2(fontSize());
}

float FontMetrics::num3() const
{
  return metricsProvider()->num3(fontSize());
}

float FontMetrics::denom1() const
{
  return metricsProvider()->denom1(fontSize());
}

float FontMetrics::denom2() const
{
  return metricsProvider()->denom2(fontSize());
}

float FontMetrics::sup1() const
{
  return metricsProvider()->sup1(fontSize());
}

float FontMetrics::sup2() const
{
  return metricsProvider()->sup2(fontSize());
}

float FontMetrics::sup3() const
{
  return metricsProvider()->sup3(fontSize());
}

float FontMetrics::sub1() const
{
  return metricsProvider()->sub1(fontSize());
}

float FontMetrics::sub2() const
{
  return metricsProvider()->sub2(fontSize());
}

float FontMetrics::supDrop() const
{
  return metricsProvider()->supDrop(fontSize());
}

float FontMetrics::subDrop() const
{
  return metricsProvider()->subDrop(fontSize());
}

float FontMetrics::delim1() const
{
  return metricsProvider()->delim1(fontSize());
}

float FontMetrics::delim2() const
{
  return metricsProvider()->delim2(fontSize());
}

float FontMetrics::axisHeight() const
{
  return metricsProvider()->axisHeight(fontSize());
}


float FontMetrics::defaultRuleThickness() const
{
  return metricsProvider()->defaultRuleThickness();
}

float FontMetrics::bigOpSpacing1() const
{
  return metricsProvider()->bigOpSpacing1();
}

float FontMetrics::bigOpSpacing2() const
{
  return metricsProvider()->bigOpSpacing2();
}

float FontMetrics::bigOpSpacing3() const
{
  return metricsProvider()->bigOpSpacing3();
}

float FontMetrics::bigOpSpacing4() const
{
  return metricsProvider()->bigOpSpacing4();
}

float FontMetrics::bigOpSpacing5() const
{
  return metricsProvider()->bigOpSpacing5();
}

} // namespace tex

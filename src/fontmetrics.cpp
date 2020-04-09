// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/fontmetrics.h"

namespace tex
{

FontMetrics::FontMetrics(Font font, std::shared_ptr<FontMetricsProdiver> mp)
  : mFont(font)
  , mMetricsProvider(mp)
{

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
  return fontdimen().slant_per_pt;
}

float FontMetrics::interwordSpace() const
{
  return fontdimen().interword_space;
}

float FontMetrics::interwordStretch() const
{
  return fontdimen().interword_stretch;
}

float FontMetrics::interwordShrink() const
{
  return fontdimen().interword_shrink;
}

float FontMetrics::extraSpace() const
{
  return fontdimen().extra_space;
}

float FontMetrics::xHeight() const
{
  return fontdimen().x_height;
}

float FontMetrics::quad() const
{
  return fontdimen().quad;
}

float FontMetrics::num1() const
{
  return fontdimen().num1;
}

float FontMetrics::num2() const
{
  return fontdimen().num2;
}

float FontMetrics::num3() const
{
  return fontdimen().num3;
}

float FontMetrics::denom1() const
{
  return fontdimen().denom1;
}

float FontMetrics::denom2() const
{
  return fontdimen().denom2;
}

float FontMetrics::sup1() const
{
  return fontdimen().sup1;
}

float FontMetrics::sup2() const
{
  return fontdimen().sup2;
}

float FontMetrics::sup3() const
{
  return fontdimen().sup3;
}

float FontMetrics::sub1() const
{
  return fontdimen().sub1;
}

float FontMetrics::sub2() const
{
  return fontdimen().sub2;
}

float FontMetrics::supDrop() const
{
  return fontdimen().sup_drop;
}

float FontMetrics::subDrop() const
{
  return fontdimen().sub_drop;
}

float FontMetrics::delim1() const
{
  return fontdimen().delim1;
}

float FontMetrics::delim2() const
{
  return fontdimen().delim2;
}

float FontMetrics::axisHeight() const
{
  return fontdimen().axis_height;
}


float FontMetrics::defaultRuleThickness() const
{
  return fontdimen().default_rule_thickness;
}

float FontMetrics::bigOpSpacing1() const
{
  return fontdimen().big_op_spacing1;
}

float FontMetrics::bigOpSpacing2() const
{
  return fontdimen().big_op_spacing2;
}

float FontMetrics::bigOpSpacing3() const
{
  return fontdimen().big_op_spacing3;
}

float FontMetrics::bigOpSpacing4() const
{
  return fontdimen().big_op_spacing4;
}

float FontMetrics::bigOpSpacing5() const
{
  return fontdimen().big_op_spacing5;
}

} // namespace tex

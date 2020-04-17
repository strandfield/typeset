// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "typeset-engine.h"

#include "tex/mathchars.h"

#include <QFontMetricsF>

#include <cmath>

RecordingFontMetricsProvider::RecordingFontMetricsProvider(FontTable & fonts)
  : QtFontMetricsProdiver(fonts)
{
 
}

float RecordingFontMetricsProvider::slantPerPt(tex::Font font)
{
  m_font_usage[font.id()].slant_per_pt = true;
  return m_fonts[font.id()].fontdimen.slant_per_pt;
}

float RecordingFontMetricsProvider::interwordSpace(tex::Font font)
{
  m_font_usage[font.id()].interword_space = true;
  return m_fonts[font.id()].fontdimen.interword_space;
}

float RecordingFontMetricsProvider::interwordStretch(tex::Font font)
{
  m_font_usage[font.id()].interword_stretch = true;
  return m_fonts[font.id()].fontdimen.interword_stretch;
}

float RecordingFontMetricsProvider::interwordShrink(tex::Font font)
{
  m_font_usage[font.id()].interword_shrink = true;
  return m_fonts[font.id()].fontdimen.interword_shrink;
}

float RecordingFontMetricsProvider::extraSpace(tex::Font font)
{
  m_font_usage[font.id()].extra_space = true;
  return m_fonts[font.id()].fontdimen.extra_space;
}


float RecordingFontMetricsProvider::xHeight(tex::Font font)
{
  m_font_usage[font.id()].x_height = true;
  return m_fonts[font.id()].fontdimen.x_height;
}

float RecordingFontMetricsProvider::quad(tex::Font font)
{
  m_font_usage[font.id()].quad = true;
  return m_fonts[font.id()].fontdimen.quad;
}

float RecordingFontMetricsProvider::num1(tex::Font font)
{
  m_font_usage[font.id()].num1 = true;
  return m_fonts[font.id()].fontdimen.num1;
}

float RecordingFontMetricsProvider::num2(tex::Font font)
{
  m_font_usage[font.id()].num2 = true;
  return m_fonts[font.id()].fontdimen.num2;
}

float RecordingFontMetricsProvider::num3(tex::Font font)
{
  m_font_usage[font.id()].num3 = true;
  return m_fonts[font.id()].fontdimen.num3;
}

float RecordingFontMetricsProvider::denom1(tex::Font font)
{
  m_font_usage[font.id()].denom1 = true;
  return m_fonts[font.id()].fontdimen.denom1;
}

float RecordingFontMetricsProvider::denom2(tex::Font font)
{
  m_font_usage[font.id()].denom2 = true;
  return m_fonts[font.id()].fontdimen.denom2;
}

float RecordingFontMetricsProvider::sup1(tex::Font font)
{
  m_font_usage[font.id()].sup1 = true;
  return m_fonts[font.id()].fontdimen.sup1;
}

float RecordingFontMetricsProvider::sup2(tex::Font font)
{
  m_font_usage[font.id()].sup2 = true;
  return m_fonts[font.id()].fontdimen.sup2;
}

float RecordingFontMetricsProvider::sup3(tex::Font font)
{
  m_font_usage[font.id()].sup3 = true;
  return m_fonts[font.id()].fontdimen.sup3;
}

float RecordingFontMetricsProvider::sub1(tex::Font font)
{
  m_font_usage[font.id()].sub1 = true;
  return m_fonts[font.id()].fontdimen.sub1;
}

float RecordingFontMetricsProvider::sub2(tex::Font font)
{
  m_font_usage[font.id()].sub2 = true;
  return m_fonts[font.id()].fontdimen.sub2;
}

float RecordingFontMetricsProvider::supDrop(tex::Font font)
{
  m_font_usage[font.id()].sup_drop = true;
  return m_fonts[font.id()].fontdimen.sup_drop;
}

float RecordingFontMetricsProvider::subDrop(tex::Font font)
{
  m_font_usage[font.id()].sub_drop = true;
  return m_fonts[font.id()].fontdimen.sub_drop;
}

float RecordingFontMetricsProvider::delim1(tex::Font font)
{
  m_font_usage[font.id()].delim1 = true;
  return m_fonts[font.id()].fontdimen.delim1;
}

float RecordingFontMetricsProvider::delim2(tex::Font font)
{
  m_font_usage[font.id()].delim2 = true;
  return m_fonts[font.id()].fontdimen.delim2;
}

float RecordingFontMetricsProvider::axisHeight(tex::Font font)
{
  m_font_usage[font.id()].axis_height = true;
  return m_fonts[font.id()].fontdimen.axis_height;
}

float RecordingFontMetricsProvider::defaultRuleThickness(tex::Font font)
{
  m_font_usage[font.id()].default_rule_thickness = true;
  return m_fonts[font.id()].fontdimen.default_rule_thickness;
}

float RecordingFontMetricsProvider::bigOpSpacing1(tex::Font font)
{
  m_font_usage[font.id()].big_op_spacing1 = true;
  return m_fonts[font.id()].fontdimen.big_op_spacing1;
}

float RecordingFontMetricsProvider::bigOpSpacing2(tex::Font font)
{
  m_font_usage[font.id()].big_op_spacing2 = true;
  return m_fonts[font.id()].fontdimen.big_op_spacing2;
}

float RecordingFontMetricsProvider::bigOpSpacing3(tex::Font font)
{
  m_font_usage[font.id()].big_op_spacing3 = true;
  return m_fonts[font.id()].fontdimen.big_op_spacing3;
}

float RecordingFontMetricsProvider::bigOpSpacing4(tex::Font font)
{
  m_font_usage[font.id()].big_op_spacing4 = true;
  return m_fonts[font.id()].fontdimen.big_op_spacing4;
}

float RecordingFontMetricsProvider::bigOpSpacing5(tex::Font font)
{
  m_font_usage[font.id()].big_op_spacing5 = true;
  return m_fonts[font.id()].fontdimen.big_op_spacing5;
}

RecordingTypesetEngine::RecordingTypesetEngine()
{
  setFontMetricsProvider<RecordingFontMetricsProvider>();
}

const std::array<FontDimenUsage, 12>& RecordingTypesetEngine::fontdimenUsage() const
{
  return static_cast<RecordingFontMetricsProvider*>(metrics().get())->m_font_usage;
}

void RecordingTypesetEngine::reset()
{
  auto& font_usage = static_cast<RecordingFontMetricsProvider*>(metrics().get())->m_font_usage;

  for (auto& f : font_usage)
  {
    f = FontDimenUsage();
  }
}
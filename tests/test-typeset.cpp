// Copyright (C) 2019 Vincent Chambrin
// This file is part of the typeset project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "test-typeset.h"

TestFontMetricsProvider::TestFontMetricsProvider()
{
  m_fontdimen.slant_per_pt = 0.f;
  m_fontdimen.interword_space = 1.f;
  m_fontdimen.interword_stretch = 1.f;
  m_fontdimen.interword_shrink = 0.1f;
  m_fontdimen.x_height = 2.f;
  m_fontdimen.quad = 3.f;
  m_fontdimen.extra_space = 1.f;
  m_fontdimen.num1 = 1.f;
  m_fontdimen.num2 = 1.f;
  m_fontdimen.num3 = 1.f;
  m_fontdimen.denom1 = 1.f;
  m_fontdimen.denom2 = 1.f;
  m_fontdimen.sup1 = 1.f;
  m_fontdimen.sup2 = 1.f;
  m_fontdimen.sup3 = 1.f;
  m_fontdimen.sub1 = 1.f;
  m_fontdimen.sub2 = 1.f;
  m_fontdimen.sup_drop = 1.f;
  m_fontdimen.sub_drop = 1.f;
  m_fontdimen.delim1 = 1.f;
  m_fontdimen.delim2 = 1.f;
  m_fontdimen.axis_height = 1.f;
  m_fontdimen.default_rule_thickness = 1.f;
  m_fontdimen.big_op_spacing1 = 1.f;
  m_fontdimen.big_op_spacing2 = 1.f;
  m_fontdimen.big_op_spacing3 = 1.f;
  m_fontdimen.big_op_spacing4 = 1.f;
  m_fontdimen.big_op_spacing5 = 1.f;
}

tex::BoxMetrics TestFontMetricsProvider::metrics(const std::shared_ptr<tex::Symbol>& symbol, tex::Font font)
{
  return tex::BoxMetrics{ 2.f, 1.f, 2.f };
}

float TestFontMetricsProvider::italicCorrection(const std::shared_ptr<tex::Symbol>& symbol, tex::Font font)
{
  return 0.0f;
}

const tex::FontDimen& TestFontMetricsProvider::fontdimen(tex::Font /* f */)
{
  return m_fontdimen;
}

TestTypesetEngine::TestTypesetEngine()
{
  m_metrics = std::make_shared<TestFontMetricsProvider>();
}

std::shared_ptr<tex::FontMetricsProvider> TestTypesetEngine::metrics() const
{
  return m_metrics;
}

std::shared_ptr<tex::Box> TestTypesetEngine::typeset(const std::string& text, tex::Font font)
{
  return std::make_shared<TestBox>(text, metrics()->metrics(nullptr, font));
}

std::shared_ptr<tex::Box> TestTypesetEngine::typeset(const std::shared_ptr<tex::Symbol>& symbol, tex::Font font)
{
  return std::make_shared<TestBox>(metrics()->metrics(symbol, font));
}

std::shared_ptr<tex::Box> TestTypesetEngine::typesetRadicalSign(float minTotalHeight)
{
  tex::BoxMetrics box;
  box.width = 2;
  box.height = metrics()->fontdimen(tex::Font::MathRoman).default_rule_thickness;
  box.depth = minTotalHeight - metrics()->fontdimen(tex::Font::MathRoman).default_rule_thickness;
  return std::make_shared<TestBox>(box);
}

std::shared_ptr<tex::Box> TestTypesetEngine::typesetDelimiter(const std::shared_ptr<tex::Symbol>& symbol, float minTotalHeight)
{
  tex::BoxMetrics box;
  box.width = 2;
  box.height = metrics()->fontdimen(tex::Font::MathRoman).default_rule_thickness;
  box.depth = minTotalHeight - metrics()->fontdimen(tex::Font::MathRoman).default_rule_thickness;
  return std::make_shared<TestBox>(box);
}

std::shared_ptr<tex::Box> TestTypesetEngine::typesetLargeOp(const std::shared_ptr<tex::Symbol>& symbol)
{
  return std::make_shared<TestBox>(metrics()->metrics(symbol, tex::Font::MathRoman));
}

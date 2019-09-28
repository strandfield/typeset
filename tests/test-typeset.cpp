// Copyright (C) 2019 Vincent Chambrin
// This file is part of the typeset project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "test-typeset.h"

tex::BoxMetrics TestFontMetricsProvider::metrics(const std::shared_ptr<tex::Symbol>& symbol, tex::Font font, tex::FontSize size)
{
  return tex::BoxMetrics{ 2.f, 1.f, 2.f };
}

float TestFontMetricsProvider::italicCorrection(const std::shared_ptr<tex::Symbol>& symbol, tex::Font font, tex::FontSize size)
{
  return 0.0f;
}

float TestFontMetricsProvider::slantPerPt(tex::Font font)
{
  return 0.0f;
}

float TestFontMetricsProvider::interwordSpace(tex::Font font)
{
  return 1.f;
}

float TestFontMetricsProvider::interwordStretch(tex::Font font)
{
  return 1.f;
}

float TestFontMetricsProvider::interwordShrink(tex::Font font)
{
  return 0.1f;
}

float TestFontMetricsProvider::extraSpace(tex::Font font)
{
  return 1.f;
}

float TestFontMetricsProvider::xHeight(tex::FontSize size)
{
  return 2.f;
}

float TestFontMetricsProvider::quad(tex::FontSize size)
{
  return 3.0f;
}

float TestFontMetricsProvider::num1(tex::FontSize size)
{
  return 1.0f;
}

float TestFontMetricsProvider::num2(tex::FontSize size)
{
  return 1.0f;
}

float TestFontMetricsProvider::num3(tex::FontSize size)
{
  return 1.0f;
}

float TestFontMetricsProvider::denom1(tex::FontSize size)
{
  return 1.0f;
}

float TestFontMetricsProvider::denom2(tex::FontSize size)
{
  return 1.0f;
}

float TestFontMetricsProvider::sup1(tex::FontSize size)
{
  return 1.0f;
}

float TestFontMetricsProvider::sup2(tex::FontSize size)
{
  return 1.0f;
}

float TestFontMetricsProvider::sup3(tex::FontSize size)
{
  return 1.0f;
}

float TestFontMetricsProvider::sub1(tex::FontSize size)
{
  return 1.0f;
}

float TestFontMetricsProvider::sub2(tex::FontSize size)
{
  return 1.0f;
}

float TestFontMetricsProvider::supDrop(tex::FontSize size)
{
  return 1.0f;
}

float TestFontMetricsProvider::subDrop(tex::FontSize size)
{
  return 1.0f;
}

float TestFontMetricsProvider::delim1(tex::FontSize size)
{
  return 1.0f;
}

float TestFontMetricsProvider::delim2(tex::FontSize size)
{
  return 1.0f;
}

float TestFontMetricsProvider::axisHeight(tex::FontSize size)
{
  return 1.0f;
}

float TestFontMetricsProvider::defaultRuleThickness()
{
  return 1.0f;
}

float TestFontMetricsProvider::bigOpSpacing1()
{
  return 1.0f;
}

float TestFontMetricsProvider::bigOpSpacing2()
{
  return 1.0f;
}

float TestFontMetricsProvider::bigOpSpacing3()
{
  return 1.0f;
}

float TestFontMetricsProvider::bigOpSpacing4()
{
  return 1.0f;
}

float TestFontMetricsProvider::bigOpSpacing5()
{
  return 1.0f;
}

TestTypesetEngine::TestTypesetEngine()
{
  m_metrics = std::make_shared<TestFontMetricsProvider>();
}

std::shared_ptr<tex::FontMetricsProdiver> TestTypesetEngine::metrics() const
{
  return m_metrics;
}

std::shared_ptr<tex::Box> TestTypesetEngine::typeset(const std::string& text, tex::Font font, tex::FontSize size)
{
  return std::make_shared<TestBox>(text, metrics()->metrics(nullptr, font, size));
}

std::shared_ptr<tex::Box> TestTypesetEngine::typeset(const std::shared_ptr<tex::Symbol>& symbol, tex::Font font, tex::FontSize size)
{
  return std::make_shared<TestBox>(metrics()->metrics(symbol, font, size));
}

std::shared_ptr<tex::Box> TestTypesetEngine::typesetRadicalSign(float minTotalHeight, tex::FontSize size)
{
  tex::BoxMetrics box;
  box.width = 2;
  box.height = metrics()->defaultRuleThickness();
  box.depth = minTotalHeight - metrics()->defaultRuleThickness();
  return std::make_shared<TestBox>(box);
}

std::shared_ptr<tex::Box> TestTypesetEngine::typesetDelimiter(const std::shared_ptr<tex::Symbol>& symbol, float minTotalHeight, tex::FontSize size)
{
  tex::BoxMetrics box;
  box.width = 2;
  box.height = metrics()->defaultRuleThickness();
  box.depth = minTotalHeight - metrics()->defaultRuleThickness();
  return std::make_shared<TestBox>(box);
}

std::shared_ptr<tex::Box> TestTypesetEngine::typesetLargeOp(const std::shared_ptr<tex::Symbol>& symbol, tex::FontSize size)
{
  return std::make_shared<TestBox>(metrics()->metrics(symbol, tex::Font::MathRoman, size));
}

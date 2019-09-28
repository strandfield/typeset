// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_TEST_TYPESET_H
#define LIBTYPESET_TEST_TYPESET_H

#include "tex/typeset.h"

class TestBox : public tex::Box
{
public:
  std::string m_text;
  tex::BoxMetrics m_metrics;

  explicit TestBox(const tex::BoxMetrics& m)
    : m_metrics(m)
  {

  }

  TestBox(std::string str, tex::BoxMetrics& m)
    : m_text(str), m_metrics(m)
  {

  }


  float height() const override { return m_metrics.height; }
  float depth() const override { return m_metrics.depth; }
  float width() const override { return m_metrics.width; }
};

class TestFontMetricsProvider : public tex::FontMetricsProdiver
{
public:
  tex::BoxMetrics metrics(const std::shared_ptr<tex::Symbol>& symbol, tex::Font font, tex::FontSize size) override;
  float italicCorrection(const std::shared_ptr<tex::Symbol>& symbol, tex::Font font, tex::FontSize size) override;

  float slantPerPt(tex::Font font) override;
  float interwordSpace(tex::Font font) override;
  float interwordStretch(tex::Font font) override;
  float interwordShrink(tex::Font font) override;
  float extraSpace(tex::Font font) override;

  float xHeight(tex::FontSize size) override;
  float quad(tex::FontSize size) override;
  float num1(tex::FontSize size) override;
  float num2(tex::FontSize size) override;
  float num3(tex::FontSize size) override;
  float denom1(tex::FontSize size) override;
  float denom2(tex::FontSize size) override;
  float sup1(tex::FontSize size) override;
  float sup2(tex::FontSize size) override;
  float sup3(tex::FontSize size) override;
  float sub1(tex::FontSize size) override;
  float sub2(tex::FontSize size) override;
  float supDrop(tex::FontSize size) override;
  float subDrop(tex::FontSize size) override;
  float delim1(tex::FontSize size) override;
  float delim2(tex::FontSize size) override;
  float axisHeight(tex::FontSize size) override;

  float defaultRuleThickness() override;
  float bigOpSpacing1() override;
  float bigOpSpacing2() override;
  float bigOpSpacing3() override;
  float bigOpSpacing4() override;
  float bigOpSpacing5() override;
};

class TestTypesetEngine : public tex::TypesetEngine
{
public:

  TestTypesetEngine();

  std::shared_ptr<tex::FontMetricsProdiver> metrics() const override;
  
  std::shared_ptr<tex::Box> typeset(const std::string& text, tex::Font font, tex::FontSize size) override;
  std::shared_ptr<tex::Box> typeset(const std::shared_ptr<tex::Symbol>& symbol, tex::Font font, tex::FontSize size)  override;
  std::shared_ptr<tex::Box> typesetRadicalSign(float minTotalHeight, tex::FontSize size)  override;
  std::shared_ptr<tex::Box> typesetDelimiter(const std::shared_ptr<tex::Symbol>& symbol, float minTotalHeight, tex::FontSize size)  override;
  std::shared_ptr<tex::Box> typesetLargeOp(const std::shared_ptr<tex::Symbol>& symbol, tex::FontSize size)  override;

private:
  std::shared_ptr<tex::FontMetricsProdiver> m_metrics;
};

#endif // LIBTYPESET_TEST_TYPESET_H

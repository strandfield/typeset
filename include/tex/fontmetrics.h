// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_FONTMETRICS_H
#define LIBTYPESET_FONTMETRICS_H

#include "tex/boxmetrics.h"
#include "tex/font.h"
#include "tex/symbol.h"

#include <memory>

namespace tex
{

struct FontPropertyUnavailable {};

class LIBTYPESET_API FontMetricsProdiver
{
public:
  FontMetricsProdiver() = default;
  virtual ~FontMetricsProdiver() = default;

  virtual BoxMetrics metrics(const std::shared_ptr<tex::Symbol> & symbol, tex::Font font, tex::FontSize size) = 0;
  virtual float italicCorrection(const std::shared_ptr<tex::Symbol> & symbol, tex::Font font, tex::FontSize size) = 0;

  virtual float slantPerPt(tex::Font font) = 0;
  virtual float interwordSpace(tex::Font font) = 0;
  virtual float interwordStretch(tex::Font font) = 0;
  virtual float interwordShrink(tex::Font font) = 0;
  virtual float extraSpace(tex::Font font) = 0;

  virtual float xHeight(tex::FontSize size) = 0;
  virtual float quad(tex::FontSize size) = 0;
  virtual float num1(tex::FontSize size) = 0;
  virtual float num2(tex::FontSize size) = 0;
  virtual float num3(tex::FontSize size) = 0;
  virtual float denom1(tex::FontSize size) = 0;
  virtual float denom2(tex::FontSize size) = 0;
  virtual float sup1(tex::FontSize size) = 0;
  virtual float sup2(tex::FontSize size) = 0;
  virtual float sup3(tex::FontSize size) = 0;
  virtual float sub1(tex::FontSize size) = 0;
  virtual float sub2(tex::FontSize size) = 0;
  virtual float supDrop(tex::FontSize size) = 0;
  virtual float subDrop(tex::FontSize size) = 0;
  virtual float delim1(tex::FontSize size) = 0;
  virtual float delim2(tex::FontSize size) = 0;
  virtual float axisHeight(tex::FontSize size) = 0;

  virtual float defaultRuleThickness() = 0;
  virtual float bigOpSpacing1() = 0;
  virtual float bigOpSpacing2() = 0;
  virtual float bigOpSpacing3() = 0;
  virtual float bigOpSpacing4() = 0;
  virtual float bigOpSpacing5() = 0;

  FontMetricsProdiver & operator=(const FontMetricsProdiver &) = delete;
};


class LIBTYPESET_API FontMetrics
{
public:
  FontMetrics(Font font, FontSize size, std::shared_ptr<FontMetricsProdiver> mp);
  FontMetrics(const FontMetrics &) = default;
  ~FontMetrics() = default;

  inline Font font() const { return mFont; }
  inline FontSize fontSize() const { return mFontSize; }
  inline const std::shared_ptr<FontMetricsProdiver> & metricsProvider() const { return mMetricsProvider; }

  BoxMetrics metrics(const std::shared_ptr<tex::Symbol> & symbol) const;
  float italicCorrection(const std::shared_ptr<tex::Symbol> & symbol) const;

  float slantPerPt() const;
  float interwordSpace() const;
  float interwordStretch() const;
  float interwordShrink() const;
  float extraSpace() const;

  float xHeight() const;
  float quad() const;
  float num1() const;
  float num2() const;
  float num3() const;
  float denom1() const;
  float denom2() const;
  float sup1() const;
  float sup2() const;
  float sup3() const;
  float sub1() const;
  float sub2() const;
  float supDrop() const;
  float subDrop() const;
  float delim1() const;
  float delim2() const;
  float axisHeight() const;

  float defaultRuleThickness() const;
  float bigOpSpacing1() const;
  float bigOpSpacing2() const;
  float bigOpSpacing3() const;
  float bigOpSpacing4() const;
  float bigOpSpacing5() const;

  FontMetrics & operator=(const FontMetrics &) = default;

private:
  Font mFont;
  FontSize mFontSize;
  std::shared_ptr<FontMetricsProdiver> mMetricsProvider;
};

} // namespace tex

#endif // LIBTYPESET_FONTMETRICS_H

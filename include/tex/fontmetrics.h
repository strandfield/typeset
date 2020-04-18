// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_FONTMETRICS_H
#define LIBTYPESET_FONTMETRICS_H

#include "tex/boxmetrics.h"
#include "tex/font.h"
#include "tex/fontdimen.h"
#include "tex/symbol.h"

#include <memory>

namespace tex
{

class LIBTYPESET_API FontMetricsProvider
{
public:
  FontMetricsProvider() = default;
  virtual ~FontMetricsProvider() = default;

  virtual BoxMetrics metrics(tex::Character c, tex::Font font) = 0;
  virtual BoxMetrics metrics(const std::shared_ptr<tex::Symbol> & symbol, tex::Font font) = 0;
  virtual float italicCorrection(const std::shared_ptr<tex::Symbol> & symbol, tex::Font font) = 0;

  virtual const FontDimen& fontdimen(Font font) = 0;

  virtual float slantPerPt(Font font);
  virtual float interwordSpace(Font font);
  virtual float interwordStretch(Font font);
  virtual float interwordShrink(Font font);
  virtual float extraSpace(Font font);

  virtual float xHeight(Font font);
  virtual float quad(Font font);
  virtual float num1(Font font);
  virtual float num2(Font font);
  virtual float num3(Font font);
  virtual float denom1(Font font);
  virtual float denom2(Font font);
  virtual float sup1(Font font);
  virtual float sup2(Font font);
  virtual float sup3(Font font);
  virtual float sub1(Font font);
  virtual float sub2(Font font);
  virtual float supDrop(Font font);
  virtual float subDrop(Font font);
  virtual float delim1(Font font);
  virtual float delim2(Font font);
  virtual float axisHeight(Font font);

  virtual float defaultRuleThickness(Font font);
  virtual float bigOpSpacing1(Font font);
  virtual float bigOpSpacing2(Font font);
  virtual float bigOpSpacing3(Font font);
  virtual float bigOpSpacing4(Font font);
  virtual float bigOpSpacing5(Font font);

  FontMetricsProvider & operator=(const FontMetricsProvider &) = delete;
};


class LIBTYPESET_API FontMetrics
{
public:
  FontMetrics(Font font, std::shared_ptr<FontMetricsProvider> mp);
  FontMetrics(const FontMetrics &) = default;
  ~FontMetrics() = default;

  inline Font font() const { return mFont; }
  inline const std::shared_ptr<FontMetricsProvider> & metricsProvider() const { return mMetricsProvider; }

  BoxMetrics metrics(tex::Character c) const;
  BoxMetrics metrics(const std::shared_ptr<tex::Symbol> & symbol) const;
  float italicCorrection(const std::shared_ptr<tex::Symbol> & symbol) const;

  const FontDimen& fontdimen() const;

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

  template<size_t I>
  float sigma() const;

  template<size_t I>
  float xi() const;

  FontMetrics & operator=(const FontMetrics &) = default;

private:
  Font mFont;
  std::shared_ptr<FontMetricsProvider> mMetricsProvider;
};

template<> inline float FontMetrics::sigma<5>() const { return xHeight(); }
template<> inline float FontMetrics::sigma<6>() const { return quad(); }
template<> inline float FontMetrics::sigma<8>() const { return num1(); }
template<> inline float FontMetrics::sigma<9>() const { return num2(); }
template<> inline float FontMetrics::sigma<10>() const { return num3(); }
template<> inline float FontMetrics::sigma<11>() const { return denom1(); }
template<> inline float FontMetrics::sigma<12>() const { return denom2(); }
template<> inline float FontMetrics::sigma<13>() const { return sup1(); }
template<> inline float FontMetrics::sigma<14>() const { return sup2(); }
template<> inline float FontMetrics::sigma<15>() const { return sup3(); }
template<> inline float FontMetrics::sigma<16>() const { return sub1(); }
template<> inline float FontMetrics::sigma<17>() const { return sub2(); }
template<> inline float FontMetrics::sigma<18>() const { return supDrop(); }
template<> inline float FontMetrics::sigma<19>() const { return subDrop(); }
template<> inline float FontMetrics::sigma<20>() const { return delim1(); }
template<> inline float FontMetrics::sigma<21>() const { return delim2(); }
template<> inline float FontMetrics::sigma<22>() const { return axisHeight(); }

template<> inline float FontMetrics::xi<8>() const { return defaultRuleThickness(); }
template<> inline float FontMetrics::xi<9>() const { return bigOpSpacing1(); }
template<> inline float FontMetrics::xi<10>() const { return bigOpSpacing2(); }
template<> inline float FontMetrics::xi<11>() const { return bigOpSpacing3(); }
template<> inline float FontMetrics::xi<12>() const { return bigOpSpacing4(); }
template<> inline float FontMetrics::xi<13>() const { return bigOpSpacing5(); }

} // namespace tex

#endif // LIBTYPESET_FONTMETRICS_H

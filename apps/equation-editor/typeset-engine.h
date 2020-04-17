// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_EQEDITOR_TYPESETENGINE_H
#define LIBTYPESET_EQEDITOR_TYPESETENGINE_H

#include "common/qt-typeset-engine.h"

struct FontDimenUsage
{
  bool slant_per_pt = false;
  bool interword_space = false;
  bool interword_stretch = false;
  bool interword_shrink = false;
  bool x_height = false;
  bool quad = false;
  bool extra_space = false;
  bool num1 = false;
  bool num2 = false;
  bool num3 = false;
  bool denom1 = false;
  bool denom2 = false;
  bool sup1 = false;
  bool sup2 = false;
  bool sup3 = false;
  bool sub1 = false;
  bool sub2 = false;
  bool sup_drop = false;
  bool sub_drop = false;
  bool delim1 = false;
  bool delim2 = false;
  bool axis_height = false;
  bool default_rule_thickness = false;
  bool big_op_spacing1 = false;
  bool big_op_spacing2 = false;
  bool big_op_spacing3 = false;
  bool big_op_spacing4 = false;
  bool big_op_spacing5 = false;
};

class RecordingFontMetricsProvider : public QtFontMetricsProdiver
{
public:
  RecordingFontMetricsProvider(FontTable& fonts);
  ~RecordingFontMetricsProvider() = default;

  float slantPerPt(tex::Font font) override;
  float interwordSpace(tex::Font font) override;
  float interwordStretch(tex::Font font) override;
  float interwordShrink(tex::Font font) override;
  float extraSpace(tex::Font font) override;

  float xHeight(tex::Font font) override;
  float quad(tex::Font font) override;
  float num1(tex::Font font) override;
  float num2(tex::Font font) override;
  float num3(tex::Font font) override;
  float denom1(tex::Font font) override;
  float denom2(tex::Font font) override;
  float sup1(tex::Font font) override;
  float sup2(tex::Font font) override;
  float sup3(tex::Font font) override;
  float sub1(tex::Font font) override;
  float sub2(tex::Font font) override;
  float supDrop(tex::Font font) override;
  float subDrop(tex::Font font) override;
  float delim1(tex::Font font) override;
  float delim2(tex::Font font) override;
  float axisHeight(tex::Font font) override;

  float defaultRuleThickness(tex::Font font) override;
  float bigOpSpacing1(tex::Font font) override;
  float bigOpSpacing2(tex::Font font) override;
  float bigOpSpacing3(tex::Font font) override;
  float bigOpSpacing4(tex::Font font) override;
  float bigOpSpacing5(tex::Font font) override;

public:
  std::array<FontDimenUsage, 12> m_font_usage;
};

class RecordingTypesetEngine : public TypesetEngine
{
public:
  RecordingTypesetEngine();

  const std::array<FontDimenUsage, 12>& fontdimenUsage() const;
  void reset();
};

#endif // LIBTYPESET_EQEDITOR_TYPESETENGINE_H

// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_EQEDITOR_TYPESETENGINE_H
#define LIBTYPESET_EQEDITOR_TYPESETENGINE_H

#include <tex/charbox.h>
#include <tex/typeset.h>
#include <tex/tfm.h>
#include <tex/math/math-typeset.h>

#include <QFont>
#include <QFontMetricsF>

#include <array>

/* File borrowed from the examples */

class QtFontMetrics : public QFontMetricsF
{
public:
  QtFontMetrics();

  using QFontMetricsF::operator=;
};

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

struct Font
{
  QString name;
  QFont font;
  QtFontMetrics metrics;
  tex::FontDimen fontdimen;
  FontDimenUsage fontdimenusage;
};

using FontTable = std::array<Font, 12>;

class QtFontMetricsProdiver : public tex::FontMetricsProvider
{
public:
  QtFontMetricsProdiver(FontTable& fonts);
  ~QtFontMetricsProdiver() = default;

  tex::BoxMetrics metrics(const std::shared_ptr<tex::Symbol> & symbol, tex::Font font) override;
  float italicCorrection(const std::shared_ptr<tex::Symbol> & symbol, tex::Font font) override;

  const tex::FontDimen& fontdimen(tex::Font f) override;

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

protected:
  const QFontMetricsF & info(tex::Font f) const;

private:
  FontTable& m_fonts;
};

class CharBox : public tex::CharacterBox
{
public:
  QFont qfont;
  QString text;
  bool deformed = false;

public:
  CharBox(tex::Character c, tex::Font f, tex::BoxMetrics box, const QFont& qf);

  void deform(const tex::BoxMetrics& boxmetrics);
};

class TypesetEngine : public tex::TypesetEngine
{
public:
  TypesetEngine();
  ~TypesetEngine() = default;

  void reset();

  const FontTable& fonts() const;

  std::array<tex::MathFont, 16> mathfonts() const;

protected:

  std::shared_ptr<tex::FontMetricsProvider> metrics() const override;

  std::shared_ptr<tex::Box> typeset(const std::string& text, tex::Font font) override;
  std::shared_ptr<tex::Box> typeset(const std::shared_ptr<tex::Symbol> & symbol, tex::Font font) override;
  std::shared_ptr<tex::Box> typesetRadicalSign(float minTotalHeight) override;
  std::shared_ptr<tex::Box> typesetDelimiter(const std::shared_ptr<tex::Symbol> & symbol, float minTotalHeight) override;
  std::shared_ptr<tex::Box> typesetLargeOp(const std::shared_ptr<tex::Symbol> & symbol) override;

  QFont& font(tex::Font f);
  void initFont(int id, const QString& displayname, const QString & fontname, int size, bool italic, tex::TFM tfm);

private:
  FontTable m_fonts;
  std::shared_ptr<QtFontMetricsProdiver> mMetrics;
  std::shared_ptr<tex::MathSymbol> mRadicalSign;
};

#endif // LIBTYPESET_EQEDITOR_TYPESETENGINE_H

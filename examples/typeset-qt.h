// Copyright (C) 2018 Vincent Chambrin
// This file is part of the liblayout project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBLAYOUT_EXAMPLES_TYPESET_QT_H
#define LIBLAYOUT_EXAMPLES_TYPESET_QT_H

#include <tex/typeset.h>

#include <QFont>
#include <QFontMetricsF>

#include <array>

using QtFontTable = std::array<std::array<QFont, 11>, 4>;

class QCharSymbol : public tex::Symbol
{
public:
  explicit QCharSymbol(QChar c) : mChar(c) { }
  ~QCharSymbol() = default;

  inline QChar character() const { return mChar; }

  static QChar get(const tex::Symbol & s);

private:
  QChar mChar;
};
//
//class QCharBox : public tex::Box
//{
//public:
//  QCharBox(QChar c, tex::BoxMetrics metrics) :
//    mChar(c),
//    mMetrics(metrics)
//  {
//
//  }
//
//  ~QCharBox() = default;
//
//  inline QChar character() const { return mChar; }
//  inline const tex::BoxMetrics & metrics() const { return mMetrics; }
//
//protected:
//  float height() const override;
//  float depth() const override;
//  float width() const override;
//
//private:
//  QChar mChar;
//  tex::BoxMetrics mMetrics;
//};

class QtFontMetrics : public QFontMetricsF
{
public:
  QtFontMetrics();

  using QFontMetricsF::operator=;
};

class QtFontMetricsProdiver : public tex::FontMetricsProdiver
{
public:
  QtFontMetricsProdiver(const QtFontTable & fonts);
  ~QtFontMetricsProdiver() = default;

  tex::BoxMetrics metrics(const std::shared_ptr<tex::Symbol> & symbol, tex::Font font, tex::FontSize size) override;
  float italicCorrection(const std::shared_ptr<tex::Symbol> & symbol, tex::Font font, tex::FontSize size) override;

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

protected:
  const QFontMetricsF & info(tex::Font f, tex::FontSize s) const;

private:
  QtFontMetrics mMetrics[4][11];
};

class QtTypesetEngine : public tex::TypesetEngine
{
public:
  QtTypesetEngine();
  ~QtTypesetEngine() = default;

protected:

  std::shared_ptr<tex::FontMetricsProdiver> metrics() const override;

  std::shared_ptr<tex::Box> typeset(const std::shared_ptr<tex::Symbol> & symbol, tex::Font font, tex::FontSize size) override;
  std::shared_ptr<tex::Box> typesetRadicalSign(float minTotalHeight, tex::FontSize size) override;
  std::shared_ptr<tex::Box> typesetDelimiter(const std::shared_ptr<tex::Symbol> & symbol, float minTotalHeight, tex::FontSize size) override;
  std::shared_ptr<tex::Box> typesetLargeOp(const std::shared_ptr<tex::Symbol> & symbol, tex::FontSize size) override;

  QFont & font(tex::Font f, tex::FontSize s);
  void initFont(tex::Font, const QString & name, int default_size, bool italic = false);

private:
  QtFontTable mFonts;
  std::shared_ptr<QtFontMetricsProdiver> mMetrics;
  std::shared_ptr<QCharSymbol> mRadicalSign;
};

#endif // LIBLAYOUT_EXAMPLES_TYPESET_QT_H

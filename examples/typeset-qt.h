// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_EXAMPLES_TYPESET_QT_H
#define LIBTYPESET_EXAMPLES_TYPESET_QT_H

#include <tex/typeset.h>

#include <QFont>
#include <QFontMetricsF>

#include <array>

using QtFontTable = std::array<QFont, 12>;

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

  tex::BoxMetrics metrics(const std::shared_ptr<tex::Symbol> & symbol, tex::Font font) override;
  float italicCorrection(const std::shared_ptr<tex::Symbol> & symbol, tex::Font font) override;

  const tex::FontDimen& fontdimen(tex::Font f) override;

protected:
  const QFontMetricsF & info(tex::Font f) const;

private:
  QtFontMetrics mMetrics[12];
  tex::FontDimen mFontDimen[12];
};

class QtTypesetEngine : public tex::TypesetEngine
{
public:
  QtTypesetEngine();
  ~QtTypesetEngine() = default;

  const QtFontTable& fonts() const;

protected:

  std::shared_ptr<tex::FontMetricsProdiver> metrics() const override;

  std::shared_ptr<tex::Box> typeset(const std::string& text, tex::Font font) override;
  std::shared_ptr<tex::Box> typeset(const std::shared_ptr<tex::Symbol> & symbol, tex::Font font) override;
  std::shared_ptr<tex::Box> typesetRadicalSign(float minTotalHeight) override;
  std::shared_ptr<tex::Box> typesetDelimiter(const std::shared_ptr<tex::Symbol> & symbol, float minTotalHeight) override;
  std::shared_ptr<tex::Box> typesetLargeOp(const std::shared_ptr<tex::Symbol> & symbol) override;

  QFont & font(tex::Font f);
  void initFont(int id, const QString & name, int size, bool italic = false);

private:
  QtFontTable mFonts;
  std::shared_ptr<QtFontMetricsProdiver> mMetrics;
  std::shared_ptr<QCharSymbol> mRadicalSign;
};

#endif // LIBTYPESET_EXAMPLES_TYPESET_QT_H

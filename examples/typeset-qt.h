// Copyright (C) 2019-2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_EXAMPLES_TYPESET_QT_H
#define LIBTYPESET_EXAMPLES_TYPESET_QT_H

#include <tex/typeset.h>
#include <tex/tfm.h>

#include <QFont>
#include <QFontMetricsF>

#include <array>

class QtFontMetrics : public QFontMetricsF
{
public:
  QtFontMetrics();

  using QFontMetricsF::operator=;
};

struct Font
{
  QFont font;
  QtFontMetrics metrics;
  tex::FontDimen fontdimen;
};

using FontTable = std::array<Font, 12>;

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

class QtFontMetricsProdiver : public tex::FontMetricsProdiver
{
public:
  QtFontMetricsProdiver(const FontTable& fonts);
  ~QtFontMetricsProdiver() = default;

  tex::BoxMetrics metrics(const std::shared_ptr<tex::Symbol> & symbol, tex::Font font) override;
  float italicCorrection(const std::shared_ptr<tex::Symbol> & symbol, tex::Font font) override;

  const tex::FontDimen& fontdimen(tex::Font f) override;

protected:
  const QFontMetricsF & info(tex::Font f) const;

private:
  FontTable m_fonts;
};

class QtTypesetEngine : public tex::TypesetEngine
{
public:
  QtTypesetEngine();
  ~QtTypesetEngine() = default;

  const FontTable& fonts() const;

protected:

  std::shared_ptr<tex::FontMetricsProdiver> metrics() const override;

  std::shared_ptr<tex::Box> typeset(const std::string& text, tex::Font font) override;
  std::shared_ptr<tex::Box> typeset(const std::shared_ptr<tex::Symbol> & symbol, tex::Font font) override;
  std::shared_ptr<tex::Box> typesetRadicalSign(float minTotalHeight) override;
  std::shared_ptr<tex::Box> typesetDelimiter(const std::shared_ptr<tex::Symbol> & symbol, float minTotalHeight) override;
  std::shared_ptr<tex::Box> typesetLargeOp(const std::shared_ptr<tex::Symbol> & symbol) override;

  QFont & font(tex::Font f);
  void initFont(int id, const QString & name, int size, bool italic, tex::TFM tfm);

private:
  FontTable m_fonts;
  std::shared_ptr<QtFontMetricsProdiver> mMetrics;
  std::shared_ptr<QCharSymbol> mRadicalSign;
};

#endif // LIBTYPESET_EXAMPLES_TYPESET_QT_H

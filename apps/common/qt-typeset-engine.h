// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_APPCOMMON_TYPESETENGINE_H
#define LIBTYPESET_APPCOMMON_TYPESETENGINE_H

#include <tex/charbox.h>
#include <tex/typeset.h>
#include <tex/tfm.h>
#include <tex/math/math-typeset.h>

#include <QFont>
#include <QFontMetricsF>
#include <QRawFont>

#include <vector>

class QtFontMetrics : public QFontMetricsF
{
public:
  QtFontMetrics();

  using QFontMetricsF::operator=;
};

struct Font
{
  QString name;
  QFont font;
  QRawFont rawfont;
  QtFontMetrics metrics;
  tex::FontDimen fontdimen;
};

using FontTable = std::vector<Font>;

class QtFontMetricsProdiver : public tex::FontMetricsProvider
{
public:
  bool frenchspacing = false;

public:
  QtFontMetricsProdiver(FontTable& fonts);
  ~QtFontMetricsProdiver() = default;

  tex::BoxMetrics metrics(tex::Character c, tex::Font font) override;
  tex::BoxMetrics metrics(const std::shared_ptr<tex::Symbol> & symbol, tex::Font font) override;
  float italicCorrection(const std::shared_ptr<tex::Symbol> & symbol, tex::Font font) override;
  int sfcode(tex::Character c) override;

  const tex::FontDimen& fontdimen(tex::Font f) override;

protected:
  const QFontMetricsF & info(tex::Font f) const;

protected:
  FontTable& m_fonts;
};

class CharBox : public tex::CharacterBox
{
public:
  QFont qfont;
  QRawFont rawfont;
  QString text;
  tex::BoxMetrics oribox;
  bool deformed = false;

public:
  CharBox(tex::Character c, tex::Font f, tex::BoxMetrics box, const QFont& qf);

  void deform(const tex::BoxMetrics& boxmetrics);
};

class TypesetEngine : public tex::TypesetEngine
{
public:
  explicit TypesetEngine(float mag = 1.f);
  ~TypesetEngine() = default;

  void reset(float mag = 1.f);

  template<typename T>
  void setFontMetricsProvider()
  {
    mMetrics = std::make_shared<T>(m_fonts);
  }

  tex::Font loadFont(const std::string fontname, const std::string& spec);

  const FontTable& fonts() const;

  std::array<tex::MathFont, 16> mathfonts() const;

public:

  std::shared_ptr<tex::FontMetricsProvider> metrics() const override;

  std::shared_ptr<tex::Box> typeset(tex::Character c, tex::Font font) override;
  std::shared_ptr<tex::Box> typeset(const std::string& text, tex::Font font) override;
  std::shared_ptr<tex::Box> typeset(const std::shared_ptr<tex::Symbol> & symbol, tex::Font font) override;
  std::shared_ptr<tex::Box> typesetRadicalSign(float minTotalHeight) override;
  std::shared_ptr<tex::Box> typesetDelimiter(const std::shared_ptr<tex::Symbol> & symbol, float minTotalHeight) override;
  std::shared_ptr<tex::Box> typesetLargeOp(const std::shared_ptr<tex::Symbol> & symbol) override;

protected:

  QFont& font(tex::Font f);
  int initFont(const QString& displayname, const QString & fontname, int size, bool italic, tex::TFM tfm);
  int initFont(const QString& displayname, const QFont& qfont, tex::TFM tfm);

private:
  float m_mag;
  FontTable m_fonts;
  std::shared_ptr<QtFontMetricsProdiver> mMetrics;
  std::shared_ptr<tex::MathSymbol> mRadicalSign;
};

#endif // LIBTYPESET_APPCOMMON_TYPESETENGINE_H

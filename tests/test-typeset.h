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

  explicit TestBox(const tex::BoxMetrics& m)
    : Box(m)
  {

  }

  TestBox(std::string str, const tex::BoxMetrics& m)
    : Box(m), m_text(str)
  {

  }
};

class TestFontMetricsProvider : public tex::FontMetricsProdiver
{
private:
  tex::FontDimen m_fontdimen;

public:

  TestFontMetricsProvider();

  tex::BoxMetrics metrics(const std::shared_ptr<tex::Symbol>& symbol, tex::Font font) override;
  float italicCorrection(const std::shared_ptr<tex::Symbol>& symbol, tex::Font font) override;

  const tex::FontDimen& fontdimen(tex::Font f) override;
};

class TestTypesetEngine : public tex::TypesetEngine
{
public:

  TestTypesetEngine();

  std::shared_ptr<tex::FontMetricsProdiver> metrics() const override;
  
  std::shared_ptr<tex::Box> typeset(const std::string& text, tex::Font font) override;
  std::shared_ptr<tex::Box> typeset(const std::shared_ptr<tex::Symbol>& symbol, tex::Font font)  override;
  std::shared_ptr<tex::Box> typesetRadicalSign(float minTotalHeight)  override;
  std::shared_ptr<tex::Box> typesetDelimiter(const std::shared_ptr<tex::Symbol>& symbol, float minTotalHeight)  override;
  std::shared_ptr<tex::Box> typesetLargeOp(const std::shared_ptr<tex::Symbol>& symbol)  override;

private:
  std::shared_ptr<tex::FontMetricsProdiver> m_metrics;
};

#endif // LIBTYPESET_TEST_TYPESET_H

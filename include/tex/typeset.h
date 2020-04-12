// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_TYPESET_H
#define LIBTYPESET_TYPESET_H

#include "tex/box.h"
#include "tex/fontmetrics.h"

namespace tex
{

namespace math
{
class Style;
} // namespace math

class LIBTYPESET_API TypesetEngine
{
public:
  TypesetEngine() = default;
  virtual ~TypesetEngine() = default;

  virtual std::shared_ptr<tex::FontMetricsProvider> metrics() const = 0;

  virtual std::shared_ptr<tex::Box> typeset(const std::string& text, tex::Font font) = 0;
  virtual std::shared_ptr<tex::Box> typeset(const std::shared_ptr<tex::Symbol> & symbol, tex::Font font) = 0;
  virtual std::shared_ptr<tex::Box> typesetRadicalSign(float minTotalHeight) = 0;
  virtual std::shared_ptr<tex::Box> typesetDelimiter(const std::shared_ptr<tex::Symbol> & symbol, float minTotalHeight) = 0;
  virtual std::shared_ptr<tex::Box> typesetLargeOp(const std::shared_ptr<tex::Symbol> & symbol) = 0;

  FontMetricsProvider & operator=(const FontMetricsProvider &) = delete;
};

class LIBTYPESET_API Options
{
public:
  explicit Options(const std::shared_ptr<TypesetEngine> & engine);
  Options(const Options &) = default;
  ~Options() = default;

  inline const std::shared_ptr<TypesetEngine> & engine() const { return mEngine; }
  math::Style mathStyle() const;
  inline Font font() const { return mFont; }
  FontMetrics fontMetrics() const;

  Options withStyle(math::Style style) const;
  Options & withStyle(math::Style style);
  Options withCrampedStyle() const;
  Options & withCrampedStyle();
  Options withFont(Font f) const;
  Options& withFont(Font f);

  Options & operator=(const Options &) = default;

private:
  std::shared_ptr<TypesetEngine> mEngine;
  int mMathStyle;
  Font mFont;
};

} // namespace tex

#endif // LIBTYPESET_TYPESET_H

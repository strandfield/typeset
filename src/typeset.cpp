// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/typeset.h"

#include "tex/math/style.h"

namespace tex
{

Options::Options(const std::shared_ptr<TypesetEngine> & engine)
  : mEngine(engine)
  , mMathStyle(math::Style::T.id())
  , mFont(Font::Default)
{

}

math::Style Options::mathStyle() const
{
  return math::Style::fromId(mMathStyle);
}

FontMetrics Options::fontMetrics() const
{
  return FontMetrics{ mFont, mEngine->metrics() };
}

Options Options::withStyle(math::Style style) const
{
  return Options{ *this }.withStyle(style);
}

Options & Options::withStyle(math::Style style)
{
  mMathStyle = style.id();
  return *this;
}

Options Options::withCrampedStyle() const
{
  return Options{ *this }.withCrampedStyle();
}

Options & Options::withCrampedStyle()
{
  mMathStyle = mathStyle().cramp().id();
  return *this;
}

Options Options::withFont(Font f) const
{
  return Options{ *this }.withFont(f);
}

Options& Options::withFont(Font f)
{
  mFont = f;
  return *this;
}

} // namespace tex

// Copyright (C) 2018 Vincent Chambrin
// This file is part of the liblayout project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/rule.h"

namespace tex
{

Rule::Rule(float w, float h, float d)
  : mWidth(w)
  , mHeight(h)
  , mDepth(d)
{

}

float Rule::height() const
{
  return mHeight;
}

float Rule::depth() const
{
  return mDepth;
}

float Rule::width() const
{
  return mWidth;
}

} // namespace tex

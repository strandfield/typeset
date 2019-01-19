// Copyright (C) 2018 Vincent Chambrin
// This file is part of the liblayout project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/math/atom.h"

namespace tex
{

namespace math
{

Atom::Atom(Type t, std::shared_ptr<Node> nucleus, std::shared_ptr<Node> subscript, std::shared_ptr<Node> superscript, std::shared_ptr<Symbol> accent, std::shared_ptr<Node> index, LimitsFlag limits)
  : mType(t)
  , mNucleus(nucleus)
  , mSubscript(subscript)
  , mSuperscript(superscript)
  , mAccent(accent)
  , mIndex(index)
  , mLimits(limits)
{

}

void Atom::changeType(Type newtype)
{
  /// TODO: check that the type change is allowed !
  mType = newtype;
}

} // namespace math

} // namespace tex

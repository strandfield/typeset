// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_PENALTY_H
#define LIBTYPESET_PENALTY_H

#include "tex/node.h"

#include <memory>

namespace tex
{

class LIBTYPESET_API Penalty final : public Node
{
public:
  Penalty(int value);
  ~Penalty() = default;

  inline int value() const { return mValue; }

  static const int Infinity = 10000;

protected:
  int mValue;
};

LIBTYPESET_API std::shared_ptr<Penalty> penalty(int p);
LIBTYPESET_API std::shared_ptr<Penalty> infinitePenalty();

} // namespace tex

#endif // LIBTYPESET_PENALTY_H

// Copyright (C) 2018 Vincent Chambrin
// This file is part of the liblayout project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBLAYOUT_PENALTY_H
#define LIBLAYOUT_PENALTY_H

#include "tex/node.h"

#include <memory>

namespace tex
{

class LIBLAYOUT_API Penalty final : public Node
{
public:
  Penalty(int value);
  ~Penalty() = default;

  inline int value() const { return mValue; }

  static const int Infinity = 10000;

protected:
  int mValue;
};

LIBLAYOUT_API std::shared_ptr<Penalty> penalty(int p);
LIBLAYOUT_API std::shared_ptr<Penalty> infinitePenalty();

} // namespace tex

#endif // LIBLAYOUT_PENALTY_H

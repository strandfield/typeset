// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_MATH_BOUNDARY_H
#define LIBTYPESET_MATH_BOUNDARY_H

#include "tex/symbol.h"

#include <memory>

namespace tex
{

namespace math
{

class LIBTYPESET_API Boundary : public Node
{
public:
  explicit Boundary(const std::shared_ptr<Symbol> & symbol) : mSymbol(symbol) { }
  ~Boundary() = default;

  inline const std::shared_ptr<Symbol> & symbol() const { return mSymbol; }

private:
  std::shared_ptr<Symbol> mSymbol;
};

} // namespace math

} // namespace tex

#endif // LIBTYPESET_MATH_BOUNDARY_H

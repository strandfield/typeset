// Copyright (C) 2018 Vincent Chambrin
// This file is part of the liblayout project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBLAYOUT_MATH_BOUNDARY_H
#define LIBLAYOUT_MATH_BOUNDARY_H

#include "tex/symbol.h"

#include <memory>

namespace tex
{

namespace math
{

class LIBLAYOUT_API Boundary : public Node
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

#endif // LIBLAYOUT_MATH_BOUNDARY_H

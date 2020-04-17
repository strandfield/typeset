// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/math/matrix.h"

namespace tex
{

namespace math
{

Matrix::Matrix(std::vector<MathList> elems, size_t cols)
  : m_cols(cols), 
    m_elements(std::move(elems))
{

}

bool Matrix::isMatrix() const
{
  return true;
}

} // namespace math

} // namespace tex

// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_MATH_MATRIX_H
#define LIBTYPESET_MATH_MATRIX_H

#include "tex/math/mathlist.h"

#include <vector>

namespace tex
{

namespace math
{

class LIBTYPESET_API Matrix : public Node
{
public:
  Matrix(std::vector<MathList> elems, size_t cols);

  bool isMatrix() const override;

  size_t cols() const;
  size_t rows() const;

  const MathList& at(size_t row, size_t col) const;
  const std::vector<MathList>& elements() const;

private:
  size_t m_cols;
  std::vector<MathList> m_elements;
};

inline size_t Matrix::cols() const
{
  return m_cols;
}

inline size_t Matrix::rows() const
{
  return elements().size() / m_cols;
}

inline const std::vector<MathList>& Matrix::elements() const
{
  return m_elements;
}

inline const MathList& Matrix::at(size_t row, size_t col) const 
{ 
  return elements().at(col + row * cols()); 
}

} // namespace math

} // namespace tex

#endif // LIBTYPESET_MATH_MATRIX_H

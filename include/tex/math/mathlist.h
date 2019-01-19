// Copyright (C) 2018 Vincent Chambrin
// This file is part of the liblayout project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBLAYOUT_MATH_LIST_H
#define LIBLAYOUT_MATH_LIST_H

#include "tex/listbox.h"

namespace tex
{

class LIBLAYOUT_API MathList : public List
{

};

class LIBLAYOUT_API MathListNode : public Node
{
public:
  MathListNode(MathList && list) : mList(std::move(list)) { }
  ~MathListNode() = default;

  inline MathList & list() { return mList; }
  inline const MathList & list() const { return mList; }

private:
  MathList mList;
};

} // namespace tex

#endif // LIBLAYOUT_MATH_LIST_H

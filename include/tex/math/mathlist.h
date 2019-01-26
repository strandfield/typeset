// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_MATH_LIST_H
#define LIBTYPESET_MATH_LIST_H

#include "tex/listbox.h"

namespace tex
{

class LIBTYPESET_API MathList : public List
{

};

class LIBTYPESET_API MathListNode : public Node
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

#endif // LIBTYPESET_MATH_LIST_H

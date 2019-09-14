// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_TYPESETTING_VISITOR_H
#define LIBTYPESET_TYPESETTING_VISITOR_H

#include "tex/defs.h"

#include "tex/symbol.h"
#include "tex/font.h"

namespace tex
{

namespace typesetting
{

class TypesetText;
class InsertInterwordGlue;
class EndParagraph;
class EnterGroup;
class LeaveGroup;
class ChangeFont;

class LIBTYPESET_API TypesettingVisitor
{
public:
  virtual ~TypesettingVisitor() = default;

  virtual void visit(TypesetText&) = 0;
  virtual void visit(InsertInterwordGlue&) = 0;
  virtual void visit(EndParagraph&) = 0;
  virtual void visit(EnterGroup&) = 0;
  virtual void visit(LeaveGroup&) = 0;
  virtual void visit(ChangeFont&) = 0;
};

} // namespace typesetting

} // namespace tex

#endif // LIBTYPESET_TYPESETTING_VISITOR_H

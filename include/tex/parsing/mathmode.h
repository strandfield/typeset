// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_PARSING_MATHMODE_H
#define LIBTYPESET_PARSING_MATHMODE_H

#include "tex/parsing/mode.h"

#include "tex/parsing/typesetting-machine.h"

#include "tex/fontmetrics.h"
#include "tex/hbox.h"
#include "tex/typeset.h"

namespace tex
{

namespace parsing
{

class LIBTYPESET_API MathMode : public Mode
{
public:
  MathMode(TypesettingMachine& m);
  ~MathMode() = default;

  TypesettingMachine& machine() const;
  FontMetrics metrics() const;

  RetCode advance() override;

  List& mlist();

private:
  List m_mlist;
};

} // namespace parsing

} // namespace tex

#endif // LIBTYPESET_PARSING_MATHMODE_H

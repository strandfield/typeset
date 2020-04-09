// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_FONTDIMEN_H
#define LIBTYPESET_FONTDIMEN_H

#include "tex/defs.h"

namespace tex
{

struct FontDimen
{
  float slant_per_pt;
  float interword_space;
  float interword_stretch;
  float interword_shrink;
  float x_height;
  float quad;
  float extra_space;
  float num1;
  float num2;
  float num3;
  float denom1;
  float denom2;
  float sup1;
  float sup2;
  float sup3;
  float sub1;
  float sub2;
  float sup_drop;
  float sub_drop;
  float delim1;
  float delim2;
  float axis_height;
  float default_rule_thickness;
  float big_op_spacing1;
  float big_op_spacing2;
  float big_op_spacing3;
  float big_op_spacing4;
  float big_op_spacing5;
};

} // namespace tex

#endif // LIBTYPESET_FONTDIMEN_H

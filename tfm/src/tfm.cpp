// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/tfm.h"

namespace tex
{

namespace tfm
{

inline static void zerofontdimen(FontDimen& fd)
{
  fd.slant_per_pt = 0.f;
  fd.interword_space = 0.f;
  fd.interword_stretch = 0.f;
  fd.interword_shrink = 0.f;
  fd.x_height = 0.f;
  fd.quad = 0.f;
  fd.extra_space = 0.f;
  fd.num1 = 0.f;
  fd.num2 = 0.f;
  fd.num3 = 0.f;
  fd.denom1 = 0.f;
  fd.denom2 = 0.f;
  fd.sup1 = 0.f;
  fd.sup2 = 0.f;
  fd.sup3 = 0.f;
  fd.sub1 = 0.f;
  fd.sub2 = 0.f;
  fd.sup_drop = 0.f;
  fd.sub_drop = 0.f;
  fd.delim1 = 0.f;
  fd.delim2 = 0.f;
  fd.axis_height = 0.f;
  fd.default_rule_thickness = 0.f;
  fd.big_op_spacing1 = 0.f;
  fd.big_op_spacing2 = 0.f;
  fd.big_op_spacing3 = 0.f;
  fd.big_op_spacing4 = 0.f;
  fd.big_op_spacing5 = 0.f;
}

const TFM& cmr10()
{
  static const TFM static_instance = []() -> TFM {

    TFM result;
    result.design_size = 10.0f;
    zerofontdimen(result.fontdimen);
    result.fontdimen.slant_per_pt = 0.f;
    result.fontdimen.interword_space = 0.333334f;
    result.fontdimen.interword_stretch = 0.166667f;
    result.fontdimen.interword_shrink = 0.111112f;
    result.fontdimen.x_height = 0.430555f;
    result.fontdimen.quad = 1.000003f;
    result.fontdimen.extra_space = 0.111112f;
    return result;

  }();

  return static_instance;
}

const TFM& cmr7()
{
  static const TFM static_instance = []() -> TFM {

    TFM result;
    result.design_size = 7.0f;
    zerofontdimen(result.fontdimen);
    result.fontdimen.slant_per_pt = 0.f;
    result.fontdimen.interword_space = 0.384923f;
    result.fontdimen.interword_stretch = 0.184525f;
    result.fontdimen.interword_shrink = 0.123016f;
    result.fontdimen.x_height = 0.430555f;
    result.fontdimen.quad = 1.138894f;
    result.fontdimen.extra_space = 0.123016f;
    return result;

  }();

  return static_instance;
}

const TFM& cmr5()
{
  static const TFM static_instance = []() -> TFM {

    TFM result;
    result.design_size = 5.0f;
    zerofontdimen(result.fontdimen);
    result.fontdimen.slant_per_pt = 0.f;
    result.fontdimen.interword_space = 0.472229f;
    result.fontdimen.interword_stretch = 0.208338f;
    result.fontdimen.interword_shrink = 0.138891f;
    result.fontdimen.x_height = 0.430554f;
    result.fontdimen.quad = 1.361133f;
    result.fontdimen.extra_space = 0.138891f;
    return result;

  }();

  return static_instance;
}

const TFM& cmmi10()
{
  static const TFM static_instance = []() -> TFM {

    TFM result;
    result.design_size = 10.0f;
    zerofontdimen(result.fontdimen);
    result.fontdimen.slant_per_pt = 0.25f;
    result.fontdimen.interword_space = 0.0f;
    result.fontdimen.interword_stretch = 0.0f;
    result.fontdimen.interword_shrink = 0.0f;
    result.fontdimen.x_height = 0.430555f;
    result.fontdimen.quad = 1.000003f;
    return result;

  }();

  return static_instance;
}

const TFM& cmmi7()
{
  static const TFM static_instance = []() -> TFM {

    TFM result;
    result.design_size = 7.0;
    zerofontdimen(result.fontdimen);
    result.fontdimen.slant_per_pt = 0.25f;
    result.fontdimen.interword_space = 0.0f;
    result.fontdimen.interword_stretch = 0.0f;
    result.fontdimen.interword_shrink = 0.0f;
    result.fontdimen.x_height = 0.430555f;
    result.fontdimen.quad = 1.170641f;
    return result;

  }();

  return static_instance;
}

const TFM& cmmi5()
{
  static const TFM static_instance = []() -> TFM {

    TFM result;
    result.design_size = 5.0;
    zerofontdimen(result.fontdimen);
    result.fontdimen.slant_per_pt = 0.25f;
    result.fontdimen.interword_space = 0.0f;
    result.fontdimen.interword_stretch = 0.0f;
    result.fontdimen.interword_shrink = 0.0f;
    result.fontdimen.x_height = 0.430554f;
    result.fontdimen.quad = 1.472241f;
    return result;

  }();

  return static_instance;
}

const TFM& cmsy10()
{
  static const TFM static_instance = []() -> TFM {

    TFM result;
    result.design_size = 10.0f;
    zerofontdimen(result.fontdimen);
    result.fontdimen.slant_per_pt = 0.25f;
    result.fontdimen.interword_space = 0.0f;
    result.fontdimen.interword_stretch = 0.0f;
    result.fontdimen.interword_shrink = 0.0f;
    result.fontdimen.x_height = 0.430555f;
    result.fontdimen.quad = 1.000003f;
    result.fontdimen.extra_space = 0.0f;
    result.fontdimen.num1 = 0.676508f;
    result.fontdimen.num2 = 0.393732f;
    result.fontdimen.num3 = 0.443731f;
    result.fontdimen.denom1 = 0.685951f;
    result.fontdimen.denom2 = 0.344841f;
    result.fontdimen.sup1 = 0.412892f;
    result.fontdimen.sup2 = 0.362892f;
    result.fontdimen.sup3 = 0.288889f;
    result.fontdimen.sub1 = 0.15f;
    result.fontdimen.sub2 = 0.247217f;
    result.fontdimen.sup_drop = 0.386108f;
    result.fontdimen.sub_drop = 0.05f;
    result.fontdimen.delim1 = 2.389999f;
    result.fontdimen.delim2 = 1.01f;
    result.fontdimen.axis_height = 0.25f;
    return result;

  }();

  return static_instance;
}

const TFM& cmsy7()
{
  static const TFM static_instance = []() -> TFM {

    TFM result;
    result.design_size = 7.0f;
    zerofontdimen(result.fontdimen);
    result.fontdimen.slant_per_pt = 0.25f;
    result.fontdimen.interword_space = 0.0f;
    result.fontdimen.interword_stretch = 0.0f;
    result.fontdimen.interword_shrink = 0.0f;
    result.fontdimen.x_height = 0.430555f;
    result.fontdimen.quad = 1.170641f;
    result.fontdimen.extra_space = 0.0f;
    result.fontdimen.num1 = 0.732313f;
    result.fontdimen.num2 = 0.383903f;
    result.fontdimen.num3 = 0.471205f;
    result.fontdimen.denom1 = 0.75215f;
    result.fontdimen.denom2 = 0.344214f;
    result.fontdimen.sup1 = 0.502543f;
    result.fontdimen.sup2 = 0.431115f;
    result.fontdimen.sup3 = 0.285714f;
    result.fontdimen.sub1 = 0.142858f;
    result.fontdimen.sub2 = 0.285714f;
    result.fontdimen.sup_drop = 0.353171f;
    result.fontdimen.sub_drop = 0.071428f;
    result.fontdimen.delim1 = 1.699999f;
    result.fontdimen.delim2 = 1.157144f;
    result.fontdimen.axis_height = 0.25f;
    return result;

  }();

  return static_instance;
}

const TFM& cmsy5()
{
  static const TFM static_instance = []() -> TFM {

    TFM result;
    result.design_size = 7.0f;
    zerofontdimen(result.fontdimen);
    result.fontdimen.slant_per_pt = 0.25f;
    result.fontdimen.interword_space = 0.0f;
    result.fontdimen.interword_stretch = 0.0f;
    result.fontdimen.interword_shrink = 0.0f;
    result.fontdimen.x_height = 0.430554f;
    result.fontdimen.quad = 1.472241f;
    result.fontdimen.extra_space = 0.0f;
    result.fontdimen.num1 = 0.925241f;
    result.fontdimen.num2 = 0.387464f;
    result.fontdimen.num3 = 0.504132f;
    result.fontdimen.denom1 = 1.025241f;
    result.fontdimen.denom2 = 0.531909f;
    result.fontdimen.sup1 = 0.503555f;
    result.fontdimen.sup2 = 0.403555f;
    result.fontdimen.sup3 = 0.294446f;
    result.fontdimen.sub1 = 0.2f;
    result.fontdimen.sub2 = 0.4f;
    result.fontdimen.sup_drop = 0.494446f;
    result.fontdimen.sub_drop = 0.1f;
    result.fontdimen.delim1 = 1.979999f;
    result.fontdimen.delim2 = 1.420001f;
    result.fontdimen.axis_height = 0.25f;
    return result;

  }();

  return static_instance;
}

const TFM& cmex10()
{
  static const TFM static_instance = []() -> TFM {

    TFM result;
    result.design_size = 10.0f;
    zerofontdimen(result.fontdimen);
    result.fontdimen.slant_per_pt = 0.0f;
    result.fontdimen.interword_space = 0.0f;
    result.fontdimen.interword_stretch = 0.0f;
    result.fontdimen.interword_shrink = 0.0f;
    result.fontdimen.x_height = 0.430555f;
    result.fontdimen.quad = 1.000003f;
    result.fontdimen.extra_space = 0.0f;
    result.fontdimen.default_rule_thickness = 0.039999f;
    result.fontdimen.big_op_spacing1 = 0.111112f;
    result.fontdimen.big_op_spacing2 = 0.166667f;
    result.fontdimen.big_op_spacing3 = 0.2f;
    result.fontdimen.big_op_spacing4 = 0.6f;
    result.fontdimen.big_op_spacing5 = 0.1f;

    return result;

  }();

  return static_instance;
}

TFM to_absolute(TFM input)
{
  const float design_size = input.design_size;
  FontDimen& fontdimen = input.fontdimen;

  fontdimen.slant_per_pt *= design_size;
  fontdimen.interword_space *= design_size;
  fontdimen.interword_stretch *= design_size;
  fontdimen.interword_shrink *= design_size;
  fontdimen.x_height *= design_size;
  fontdimen.quad *= design_size;
  fontdimen.extra_space *= design_size;
  fontdimen.num1 *= design_size;
  fontdimen.num2 *= design_size;
  fontdimen.num3 *= design_size;
  fontdimen.denom1 *= design_size;
  fontdimen.denom2 *= design_size;
  fontdimen.sup1 *= design_size;
  fontdimen.sup2 *= design_size;
  fontdimen.sup3 *= design_size;
  fontdimen.sub1 *= design_size;
  fontdimen.sub2 *= design_size;
  fontdimen.sup_drop *= design_size;
  fontdimen.sub_drop *= design_size;
  fontdimen.delim1 *= design_size;
  fontdimen.delim2 *= design_size;
  fontdimen.axis_height *= design_size;
  fontdimen.default_rule_thickness *= design_size;
  fontdimen.big_op_spacing1 *= design_size;
  fontdimen.big_op_spacing2 *= design_size;
  fontdimen.big_op_spacing3 *= design_size;
  fontdimen.big_op_spacing4 *= design_size;
  fontdimen.big_op_spacing5 *= design_size;

  return input;
}

TFM to_relative(TFM input)
{
  const float design_size = input.design_size;
  FontDimen& fontdimen = input.fontdimen;

  fontdimen.slant_per_pt /= design_size;
  fontdimen.interword_space /= design_size;
  fontdimen.interword_stretch /= design_size;
  fontdimen.interword_shrink /= design_size;
  fontdimen.x_height /= design_size;
  fontdimen.quad /= design_size;
  fontdimen.extra_space /= design_size;
  fontdimen.num1 /= design_size;
  fontdimen.num2 /= design_size;
  fontdimen.num3 /= design_size;
  fontdimen.denom1 /= design_size;
  fontdimen.denom2 /= design_size;
  fontdimen.sup1 /= design_size;
  fontdimen.sup2 /= design_size;
  fontdimen.sup3 /= design_size;
  fontdimen.sub1 /= design_size;
  fontdimen.sub2 /= design_size;
  fontdimen.sup_drop /= design_size;
  fontdimen.sub_drop /= design_size;
  fontdimen.delim1 /= design_size;
  fontdimen.delim2 /= design_size;
  fontdimen.axis_height /= design_size;
  fontdimen.default_rule_thickness /= design_size;
  fontdimen.big_op_spacing1 /= design_size;
  fontdimen.big_op_spacing2 /= design_size;
  fontdimen.big_op_spacing3 /= design_size;
  fontdimen.big_op_spacing4 /= design_size;
  fontdimen.big_op_spacing5 /= design_size;

  return input;
}

} // namespace tfm

} // namespace tex

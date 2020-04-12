// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_TFM_H
#define LIBTYPESET_TFM_H

#include "tex/fontdimen.h"

namespace tex
{

struct TFM
{
  float design_size;
  FontDimen fontdimen;
};

namespace tfm
{

// \textfont0
const TFM& cmr10();
// \scriptfont0
const TFM& cmr7();
// \scriptscriptfont0
const TFM& cmr5();

// \textfont1
const TFM& cmmi10();
// \scriptfont1
const TFM& cmmi7();
// \scriptscriptfont1
const TFM& cmmi5();

// \textfont2
const TFM& cmsy10();
// \scriptfont2
const TFM& cmsy7();
// \scriptscriptfont2
const TFM& cmsy5();

// \textfont3 \scriptfont3 \scriptscriptfont3
const TFM& cmex10();

TFM to_absolute(TFM input);
TFM to_relative(TFM input);

} // namespace tfm

} // namespace tex

#endif // LIBTYPESET_TFM_H

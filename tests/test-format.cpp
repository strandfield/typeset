// Copyright (C) 2020 Vincent Chambrin
// This file is part of the typeset project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "test.h"

#include "tex/parsing/format.h"

using namespace tex;

const char* fmt_src =
  "" // 0
  "\\def\\begin#1{"
  "  \\csname #1 \\endcsname"
  "}"
  ""
  "\\def\\end#1{" // 5
  "  \\csname end#1\\endcsname "
  "}"
  ""
;

const char* fmt_bytecode =
  "v0"
  "" // 0
  "d5pr"
  ">2c6>1p>1c9"
  "s"
  ""
  "d3pr" // 5
  ">2c6>1tttpc9>1"
  "s"
  ""
  "e";


TEST_CASE(test_format_begin_end)
{
 using namespace parsing;

 auto result = Format::load(Format(fmt_src, fmt_bytecode));

 ASSERT(result.size() == 2);

 const Macro& begin = result.front();

 ASSERT_EQ(begin.controlSequence(), "begin");
 ASSERT_EQ(begin.parameterText().size(), 1);
 ASSERT_EQ(begin.replacementText().size(), 3);

 const Macro& end = result.back();

 ASSERT_EQ(end.controlSequence(), "end");
 ASSERT_EQ(end.parameterText().size(), 1);
 ASSERT_EQ(end.replacementText().size(), 6);
}

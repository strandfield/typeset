// Copyright (C) 2020 Vincent Chambrin
// This file is part of the typeset project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "catch.hpp"

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


TEST_CASE("Formats can be loaded", "[format]")
{
 using namespace parsing;

 auto result = Format::load(Format(fmt_src, fmt_bytecode));

 REQUIRE(result.size() == 2);

 const Macro& begin = result.front();

 REQUIRE(begin.controlSequence() == "begin");
 REQUIRE(begin.parameterText().size() == 1);
 REQUIRE(begin.replacementText().size() == 3);

 const Macro& end = result.back();

 REQUIRE(end.controlSequence() == "end");
 REQUIRE(end.parameterText().size() == 1);
 REQUIRE(end.replacementText().size() == 6);
}

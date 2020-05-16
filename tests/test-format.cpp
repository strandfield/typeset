// Copyright (C) 2020 Vincent Chambrin
// This file is part of the typeset project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "catch.hpp"

#include "tex/parsing/format.h"

using namespace tex;

TEST_CASE("A simple format can be parsed", "[format]")
{
 using namespace parsing;

 std::string src =
   "\\def\\begin#1{%\n"
   "  \\csname #1\\endcsname\n"
   "}\n"
   "\n"
   "\\def\\end#1{%\n"
   "  \\csname end#1\\endcsname\n"
   "}\n";

 auto result = Format::parse(src);

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

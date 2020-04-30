// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "pageeditor-format.h"

const char* pageeditor_fmt_src =
  "" // 0
  "\\def\\begin#1{\\csname #1\\endcsname}"
  ""
  "\\def\\end#1{\\csname end#1\\endcsname}"
  ""
  ;

const char* pageeditor_fmt_bytecode =
  "v0"
  "" // 0
  "d5prc6>1pc9s"
  ""
  "d3prc6>1tttpc9s"
  ""
  "e";

tex::parsing::Format pageeditor_format()
{
  return tex::parsing::Format(pageeditor_fmt_src, pageeditor_fmt_bytecode);
}

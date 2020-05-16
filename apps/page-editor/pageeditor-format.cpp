// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "pageeditor-format.h"

std::string pageeditor_format()
{
  return 
    "\\def\\begin#1{\\csname #1\\endcsname}\n"
    "\\def\\end#1{\\csname end#1\\endcsname}";
}

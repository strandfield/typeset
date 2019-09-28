// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/parsing/inputstream.h"

#include <fstream>
#include <sstream>

namespace tex
{

namespace parsing
{

static void removeCRLF(std::string& str)
{
  size_t w = 0;

  for (size_t r(0); r < str.size(); ++r)
  {
    if (str[r] != '\r')
    {
      str[w++] = str[r];
    }
  }

  str.resize(w);
}

InputStream::Document InputStream::Document::fromString(std::string&& str)
{
  Document ret;
  ret.content = std::move(str);
 
  removeCRLF(ret.content);

  return ret;
}

InputStream::Document InputStream::Document::fromLocalFile(const std::string& path)
{
  std::ifstream file{ path };

  if (!file.is_open())
  {
    return {};
  }

  std::ifstream t("file.txt");
  std::stringstream buffer;
  buffer << t.rdbuf();

  Document ret;
  ret.filepath = path;
  ret.content = buffer.str();

  removeCRLF(ret.content);

  return ret;
}

} // namespace parsing

} // namespace tex

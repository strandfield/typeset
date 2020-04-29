// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_PARSING_MATHPARSERFRONTEND_H
#define LIBTYPESET_PARSING_MATHPARSERFRONTEND_H

#include "tex/parsing/mathparser.h"

#include "tex/mathcode.h"

namespace tex
{

namespace parsing
{

class LIBTYPESET_API MathParserFrontend
{
public:
  MathParserFrontend();

  MathParser& parser();
  int fam() const;

  enum class CS
  {
    LEFT,
    RIGHT,
    OVER,
    FRAC,
    SQRT,
    MATRIX,
    CR,
    TEXTSTYLE,
    SCRIPTSTYLE,
    SCRIPTSCRIPTSTYLE,
  };

  static const std::map<std::string, CS>& csmap();
  static CS cs(const std::string& name);

  static const std::map<std::string, std::pair<int, MathCode>>& symbolsmap();

  void writeControlSequence(CS cs);
  void writeControlSequence(const std::string& csname);

  void writeChar(char c);

  // @TODO: maybe not that good, should be protected maybe 
  void writeSymbol(Character c);
  void writeMathChar(Character c, MathCode mc);

  void writeSymbol(std::shared_ptr<MathSymbol> mathsym);

  void writeBox(const std::shared_ptr<tex::Box>& box);

  void beginSuperscript();
  void beginSubscript();

  void beginMathList();
  void endMathList();

  void alignmentTab();

  void finish();

  MathList& output();

protected:
  void writeSymbol(Character c, int fam, int class_num);

private:
  MathParser m_parser;
  MathCode::Table m_mathcode_table;
  int m_fam = -1;
};

} // namespace parsing

} // namespace tex

#endif // LIBTYPESET_PARSING_MATHPARSERFRONTEND_H

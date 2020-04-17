// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/parsing/mathparserfrontend.h"

#include "tex/math/stylechange.h"

#include "tex/mathchars.h"

#include <cassert>
#include <stdexcept>

namespace tex
{

namespace parsing
{

MathParserFrontend::MathParserFrontend()
  : m_mathcode_table{MathCode::DefaultTable}
{
}

MathParser& MathParserFrontend::parser()
{
  return m_parser;
}

int MathParserFrontend::fam() const
{
  return m_fam;
}

const std::map<std::string, MathParserFrontend::CS>& MathParserFrontend::csmap()
{
  static const std::map<std::string, MathParserFrontend::CS> map = {
    {"left", CS::LEFT},
    {"right", CS::RIGHT},
    {"over", CS::OVER},
    {"frac", CS::FRAC},
    {"sqrt", CS::SQRT},
    {"matrix", CS::MATRIX},
    {"cr", CS::CR},
    {"textstyle", CS::TEXTSTYLE},
    {"scriptstyle", CS::SCRIPTSTYLE},
    {"scriptscriptstyle", CS::SCRIPTSCRIPTSTYLE},
    /* Symbols */
    {"bullet", CS::BULLET},
    {"cap", CS::CAP},
    {"cdot", CS::CDOT},
    {"circ", CS::CIRC},
    {"cup", CS::CUP},
    {"sqcap", CS::SQCAP},
    {"sqcup", CS::SQCUP},
    {"times", CS::TIMES},
  };

  return map;
}

MathParserFrontend::CS MathParserFrontend::cs(const std::string& name)
{
  auto it = csmap().find(name);

  if (it == csmap().end())
    throw std::runtime_error{ "Unknown control sequence" };

  return it->second;
}

void MathParserFrontend::writeControlSequence(CS cs)
{
  switch (cs)
  {
  case CS::LEFT:
    return parser().left();
  case CS::RIGHT:
    return parser().right();
  case CS::OVER:
    return parser().over();
  case CS::FRAC:
    return parser().frac();
  case CS::SQRT:
    return parser().sqrt();
  case CS::MATRIX:
    return parser().matrix();
  case CS::CR:
    return parser().cr();
  case CS::TEXTSTYLE:
    return parser().textstyle();
  case CS::SCRIPTSTYLE:
    return parser().scriptstyle();
  case CS::SCRIPTSCRIPTSTYLE:
    return parser().scriptscriptstyle();
    /* Symbols */
  case CS::BULLET:
    return writeSymbol(mathchars::BULLET, math::Atom::Bin, 2);
  case CS::CAP:
    return writeSymbol(mathchars::CAP, math::Atom::Bin, 2);
  case CS::CDOT:
    return writeSymbol(mathchars::CDOT, math::Atom::Bin, 2);
  case CS::CIRC:
    return writeSymbol(mathchars::CIRC, math::Atom::Bin, 2);
  case CS::CUP:
    return writeSymbol(mathchars::CUP, math::Atom::Bin, 2);
  case CS::SQCAP:
    return writeSymbol(mathchars::SQCAP, math::Atom::Bin, 2);
  case CS::SQCUP:
    return writeSymbol(mathchars::SQCUP, math::Atom::Bin, 2);
  case CS::TIMES:
    return writeSymbol(mathchars::TIMES, math::Atom::Bin, 2);
  }
}

void MathParserFrontend::writeControlSequence(const std::string& csname)
{
  CS control_seq = cs(csname);
  writeControlSequence(control_seq);
}

void MathParserFrontend::writeChar(char c)
{
  MathCode mc = m_mathcode_table[static_cast<uint8_t>(c)];

  int class_num = mc.c();
  int f = mc.f();

  if (class_num == 7)
  {
    class_num = 0;
    f = fam() != -1 ? fam() : f;
  }

  auto mathsym = std::make_shared<MathSymbol>(Character(c), class_num, f);
  parser().writeSymbol(mathsym);
}

void MathParserFrontend::writeSymbol(Character c)
{
  if (c < 128)
  {
    writeChar(c);
  }
  else
  {
    throw std::runtime_error{ "MathParserFrontend does not support unicode char yet" };
  }
}

void MathParserFrontend::writeSymbol(Character c, int class_num, int fam)
{
  auto mathsym = std::make_shared<MathSymbol>(Character(c), class_num, fam);
  parser().writeSymbol(mathsym);
}

void MathParserFrontend::writeSymbol(std::shared_ptr<MathSymbol> mathsym)
{
  parser().writeSymbol(mathsym);
}

void MathParserFrontend::writeBox(const std::shared_ptr<tex::Box>& box)
{
  parser().writeBox(box);
}

void MathParserFrontend::beginSuperscript()
{
  parser().beginSuperscript();
}

void MathParserFrontend::beginSubscript()
{
  parser().beginSubscript();
}

void MathParserFrontend::beginMathList()
{
  parser().beginMathList();
}

void MathParserFrontend::endMathList()
{
  parser().endMathList();
}

void MathParserFrontend::alignmentTab()
{
  parser().alignmentTab();
}

void MathParserFrontend::finish()
{
  parser().finish();
}

MathList& MathParserFrontend::output()
{
  return parser().output();
}

} // namespace parsing

} // namespace tex

// Copyright (C) 2019-2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/parsing/mathparser.h"

#include "tex/math/boundary.h"
#include "tex/math/fontchange.h"
#include "tex/math/fraction.h"
#include "tex/math/math-typeset.h"
#include "tex/math/mathlist.h"
#include "tex/math/matrix.h"
#include "tex/math/root.h"
#include "tex/math/stylechange.h"

#include "tex/mathchars.h"

#include <cassert>
#include <stdexcept>

namespace tex
{

namespace parsing
{

AtomBuilder::AtomBuilder()
{

}

AtomBuilder::AtomBuilder(math::Atom::Type t)
  : type(t)
{

}

AtomBuilder& AtomBuilder::setNucleus(const std::shared_ptr<Node>& node)
{
  nucleus_ = node;
  return *this;
}

AtomBuilder& AtomBuilder::setSuperscript(const std::shared_ptr<Node>& node)
{
  superscript_ = node;
  return *this;
}

AtomBuilder& AtomBuilder::setSubscript(const std::shared_ptr<Node>& node)
{
  subscript_ = node;
  return *this;
}

std::shared_ptr<math::Atom> AtomBuilder::build() const
{
  return std::make_shared<math::Atom>(type, nucleus(), subscript(), superscript(), nullptr, math::Atom::NoLimits);
}

MathList& MatrixBuilder::Row::newCell()
{
  auto node = std::make_shared<MathListNode>();
  cells.push_back(node);
  return node->list();
}

MatrixBuilder::Row& MatrixBuilder::newRow()
{
  rows.emplace_back();
  return lastRow();
}

MatrixBuilder::Row& MatrixBuilder::lastRow()
{
  return rows.back();
}

std::shared_ptr<Node> MatrixBuilder::build() const
{
  size_t nb_cols = 0;

  for (const Row& r : rows)
  {
    nb_cols = std::max({ r.cells.size(), nb_cols });
  }

  std::vector<MathList> elements;

  for (const Row& r : rows)
  {
    for (auto node : r.cells)
      elements.push_back(node->list());

    for (size_t i(0); i < (nb_cols - r.cells.size()); ++i)
      elements.emplace_back();
  }

  return std::make_shared<math::Matrix>(std::move(elements), nb_cols);
}

MathParser::MathParser()
{
  enter(State::ParsingMList);
}

MathList& MathParser::mlist()
{
  return m_lists.empty() ? m_mlist : *m_lists.back();
}

MatrixBuilder& MathParser::currentMatrix()
{
  return m_matrices.back();
}

void MathParser::popMatrix()
{
  m_matrices.pop_back();
}

void MathParser::writeSymbol(std::shared_ptr<MathSymbol> mathsym)
{
  switch (state())
  {
  case State::ParsingMList:
    return parse_mlist(mathsym);
  case State::ParsingAtom:
    return parse_atom(mathsym);
  case State::ParsingNucleus:
  case State::ParsingSubscript:
  case State::ParsingSuperscript:
    return parse_mlist(mathsym);
  case State::AwaitingSubscript:
  case State::AwaitingSuperscript:
    return parse_subsupscript(mathsym);
  case State::ParsingBoundary:
    return parse_mlist(mathsym);
  case State::ParsingLeft:
    return parse_left(mathsym);
  case State::ParsingRight:
    return parse_right(mathsym);
  case State::ParsingSqrt:
    return parse_sqrt(mathsym);
  case State::ParsingSqrtDegree:
    return parse_sqrt_degree(mathsym);
  case State::ParsingSqrtRadicand:
    return parse_sqrt_radicand(mathsym);
  case State::ParsingSqrtRadicandMList:
    return parse_mlist(mathsym);
  case State::ParsingOver:
    return parse_mlist(mathsym);
  case State::ParsingFracDenom:
    return parse_frac_denom(mathsym);
  case State::ParsingFracNumer:
    return parse_frac_numer(mathsym);
  case State::ParsingFracDenomMList:
    return parse_mlist(mathsym);
  case State::ParsingFracNumerMList:
    return parse_mlist(mathsym);
  case State::ParsingMatrix:
    throw std::runtime_error{ "Expected '{' after \\matrix" };
  case State::ParsingMatrixCell:
    return parse_mlist(mathsym);
  default:
    throw std::runtime_error{ "Bad call to writeSymbol()" };
  }
}

void MathParser::writeBox(const std::shared_ptr<tex::Box>& box)
{
  throw std::runtime_error{ "Not implemented" };
}

void MathParser::beginSuperscript()
{
  if (isParsingMList())
  {
    m_builders.emplace_back();
    enter(State::ParsingAtom);
  }

  switch (state())
  {
  case State::ParsingAtom:
    enter(State::AwaitingSuperscript);
    break;
  default:
    throw std::runtime_error{ "Invalid call to beginSuperscript()" };
  }
}

void MathParser::beginSubscript()
{
  if (isParsingMList())
  {
    m_builders.emplace_back();
    enter(State::ParsingAtom);
  }

  switch (state())
  {
  case State::ParsingAtom:
    enter(State::AwaitingSubscript);
    break;
  default:
    throw std::runtime_error{ "Invalid call to beginSubscript()" };
  }
}

void MathParser::beginMathList()
{
  if (state() == State::ParsingSqrt)
  {
    leaveState();

    enter(State::ParsingAtom);
    m_builders.emplace_back(AtomBuilder(math::Atom::Rad));
    enter(State::ParsingNucleus);
    m_builders.back().setNucleus(pushMathList());
    return;
  }
  else if (state() == State::ParsingSqrtRadicand)
  {
    auto root = std::dynamic_pointer_cast<math::Root>(mlist().back());
    enter(State::ParsingSqrtRadicandMList);
    pushList(root->radicand());
    return;
  }
  else if (state() == State::ParsingFracNumer)
  {
    auto frac = std::dynamic_pointer_cast<math::Fraction>(mlist().back());
    enter(State::ParsingFracNumerMList);
    pushList(frac->numer());
    return;
  }
  else if (state() == State::ParsingFracDenom)
  {
    auto frac = std::dynamic_pointer_cast<math::Fraction>(mlist().back());
    enter(State::ParsingFracDenomMList);
    pushList(frac->denom());
    return;
  }
  else if (state() == State::ParsingMatrix)
  {
    enter(State::ParsingMatrixCell);
    MatrixBuilder builder;
    pushList(builder.newRow().newCell());
    m_matrices.push_back(std::move(builder));
    return;
  }

  if (state() == State::AwaitingSubscript)
  {
    leaveState();
    enter(State::ParsingSubscript);
    m_builders.back().setSuperscript(pushMathList());
  }
  else if (state() == State::AwaitingSuperscript)
  {
    leaveState();
    enter(State::ParsingSuperscript);
    m_builders.back().setSuperscript(pushMathList());
  }
  else
  {
    commitCurrentAtom();

    enter(State::ParsingAtom);
    m_builders.emplace_back();
    enter(State::ParsingNucleus);
    m_builders.back().setNucleus(pushMathList());
  }
}

void MathParser::endMathList()
{
  commitCurrentAtom();

  switch (state())
  {
  case State::ParsingNucleus:
  case State::ParsingSubscript:
  case State::ParsingSuperscript:
    popList();
    leave(state());
    break;
  case State::ParsingOver:
  {
    popList();
    leaveState();

    if (state() == State::ParsingNucleus || state() == State::ParsingSubscript || state() == State::ParsingSuperscript)
    {
      endMathList();
    }

    break;
  }
  case State::ParsingFracNumerMList:
  {
    popList();
    leave(State::ParsingFracNumerMList);
    leave(State::ParsingFracNumer);
    enter(State::ParsingFracDenom);
    break;
  }
  case State::ParsingFracDenomMList:
  {
    popList();
    leave(State::ParsingFracDenomMList);
    leave(State::ParsingFracDenom);
    leave(State::ParsingFrac);
    break;
  }
  case State::ParsingSqrtRadicandMList:
  {
    popList();
    leave(State::ParsingSqrtRadicandMList);
    leave(State::ParsingSqrtRadicand);
    leave(State::ParsingSqrt);
    break;
  }
  case State::ParsingMatrixCell:
  {
    leaveState();
    leave(State::ParsingMatrix);
    popList();
    auto node = currentMatrix().build();
    popMatrix();
    mlist().push_back(node);
    break;
  }
  default:
    throw std::runtime_error{ "Bad call to endMathList()" };
  }
}

void MathParser::finish()
{
  commitCurrentAtom();

  if (state() == State::ParsingOver)
  {
    popList();
    leaveState();
  }

  if (state() != State::ParsingMList)
    throw std::runtime_error{ "Missing ending '}'" };
}

MathList& MathParser::output()
{
  return m_mlist;
}

void MathParser::commitCurrentAtom()
{
  if (state() == State::ParsingAtom)
  {
    auto atom = m_builders.back().build();
    m_builders.pop_back();
    leave(State::ParsingAtom);
    mlist().push_back(atom);
  }
}

void MathParser::enter(State s)
{
  m_states.push_back(s);
}

void MathParser::leave(State s)
{
  assert(state() == s);
  leaveState();
}

void MathParser::leaveState()
{
  m_states.pop_back();
}

void MathParser::pushList(MathList& l)
{
  m_lists.push_back(&l);
}

void MathParser::popList()
{
  m_lists.pop_back();
}

std::shared_ptr<MathListNode> MathParser::pushMathList()
{
  auto ret = std::make_shared<MathListNode>();
  pushList(ret->list());
  return ret;
}

bool MathParser::isParsingMList() const
{
  switch (state())
  {
  case State::ParsingMList:
  case State::ParsingNucleus:
  case State::ParsingSubscript:
  case State::ParsingSuperscript:
  case State::ParsingSqrtDegree:
  case State::ParsingBoundary:
  case State::ParsingSqrtRadicandMList:
  case State::ParsingOver:
  case State::ParsingFracDenomMList:
  case State::ParsingFracNumerMList:
    return true;
  default:
    return false;
  }
}

void MathParser::parse_mlist(std::shared_ptr<MathSymbol> mathsym)
{
  enter(State::ParsingAtom);
  m_builders.emplace_back();
  m_builders.back().type = static_cast<math::Atom::Type>(mathsym->classNumber());
  m_builders.back().setNucleus(mathsym);
}

void MathParser::parse_atom(std::shared_ptr<MathSymbol> mathsym)
{
  commitCurrentAtom();
  return writeSymbol(mathsym);
}

void MathParser::parse_subsupscript(std::shared_ptr<MathSymbol> mathsym)
{
  if (state() == State::AwaitingSubscript)
  {
    m_builders.back().setSubscript(mathsym);
    leaveState();
  }
  else if (state() == State::AwaitingSuperscript)
  {
    m_builders.back().setSuperscript(mathsym);
    leaveState();
  }
}

void MathParser::parse_left(std::shared_ptr<MathSymbol> mathsym)
{
  mlist().push_back(std::make_shared<math::Boundary>(mathsym));
  leave(State::ParsingLeft);
  assert(state() == State::ParsingBoundary);
}

void MathParser::parse_right(std::shared_ptr<MathSymbol> mathsym)
{
  mlist().push_back(std::make_shared<math::Boundary>(mathsym));
  leave(State::ParsingRight);
  leave(State::ParsingBoundary);
  leave(State::ParsingNucleus);
  popList();
  assert(state() == State::ParsingAtom);
}

void MathParser::parse_sqrt(std::shared_ptr<MathSymbol> mathsym)
{
  if (mathsym->character() == '[')
  {
    enter(State::ParsingSqrtDegree);
    auto root = std::make_shared<math::Root>();
    mlist().push_back(root);
    pushList(root->degree());
  }
  else
  {
    leave(State::ParsingSqrt);

    m_builders.emplace_back(AtomBuilder(math::Atom::Rad).setNucleus(mathsym));
    enter(State::ParsingAtom);
  }
}

void MathParser::parse_sqrt_degree(std::shared_ptr<MathSymbol> mathsym)
{
  if (mathsym->character() == ']')
  {
    leave(State::ParsingSqrtDegree);
    popList();
    enter(State::ParsingSqrtRadicand);
  }
  else
  {
    parse_mlist(mathsym);
  }
}

void MathParser::parse_sqrt_radicand(std::shared_ptr<MathSymbol> mathsym)
{
  AtomBuilder builder{ math::Atom::Ord }; // @TODO: is-it Ord ?
  builder.setNucleus(mathsym);

  auto root = std::dynamic_pointer_cast<math::Root>(mlist().back());
  root->radicand().push_back(builder.build());

  leave(State::ParsingSqrtRadicand);
  leave(State::ParsingSqrt);
}

void MathParser::parse_frac_numer(std::shared_ptr<MathSymbol> mathsym)
{
  AtomBuilder builder{ math::Atom::Ord }; // @TODO: is-it Ord ?
  builder.setNucleus(mathsym);

  auto frac = std::dynamic_pointer_cast<math::Fraction>(mlist().back());
  frac->numer().push_back(builder.build());

  leave(State::ParsingFracNumer);
  enter(State::ParsingFracDenom);
}

void MathParser::parse_frac_denom(std::shared_ptr<MathSymbol> mathsym)
{
  AtomBuilder builder{ math::Atom::Ord }; // @TODO: is-it Ord ?
  builder.setNucleus(mathsym);

  auto frac = std::dynamic_pointer_cast<math::Fraction>(mlist().back());
  frac->denom().push_back(builder.build());

  leave(State::ParsingFracDenom);
  leave(State::ParsingFrac);
}

void MathParser::left()
{
  if (state() == State::ParsingAtom)
    commitCurrentAtom();

  m_builders.emplace_back(AtomBuilder(math::Atom::Inner).setNucleus(pushMathList()));
  enter(State::ParsingAtom);
  enter(State::ParsingNucleus);
  enter(State::ParsingBoundary);
  enter(State::ParsingLeft);
}

void MathParser::right()
{
  if (state() == State::ParsingAtom)
    commitCurrentAtom();

  if (state() != State::ParsingBoundary)
  {
    throw std::runtime_error{ "Mismatching \\left & \\right" };
  }

  enter(State::ParsingRight);
}

void MathParser::over()
{
  if (state() == State::ParsingAtom)
    commitCurrentAtom();

  MathList& ml = mlist();
  auto frac = std::make_shared<math::Fraction>(std::move(ml), MathList{});
  ml.clear();
  ml.push_back(frac);
  enter(State::ParsingOver);
  pushList(frac->denom());
}

void MathParser::frac()
{
  if (state() == State::ParsingAtom)
    commitCurrentAtom();

  auto frac = std::make_shared<math::Fraction>();
  mlist().push_back(frac);
  enter(State::ParsingFrac);
  enter(State::ParsingFracNumer);
}

void MathParser::sqrt()
{
  if (state() == State::ParsingAtom)
    commitCurrentAtom();

  enter(State::ParsingSqrt);
}

void MathParser::matrix()
{
  if (state() == State::ParsingAtom)
    commitCurrentAtom();

  enter(State::ParsingMatrix);
}

void MathParser::alignmentTab()
{
  commitCurrentAtom();

  if (state() != State::ParsingMatrixCell)
    throw std::runtime_error{ "MathParser::alignmentTab() can only be called when parsing a matrix" };

  popList();
  pushList(currentMatrix().lastRow().newCell());
}

void MathParser::cr()
{
  commitCurrentAtom();

  if (state() != State::ParsingMatrixCell)
    throw std::runtime_error{ "MathParser::cr() can only be called when parsing a matrix" };

  popList();
  pushList(currentMatrix().newRow().newCell());
}

void MathParser::textstyle()
{
  if (state() == State::ParsingAtom)
    commitCurrentAtom();

  mlist().push_back(std::make_shared<math::StyleChange>(math::Style::T));
}

void MathParser::scriptstyle()
{
  if (state() == State::ParsingAtom)
    commitCurrentAtom();

  mlist().push_back(std::make_shared<math::StyleChange>(math::Style::S));
}

void MathParser::scriptscriptstyle()
{
  if (state() == State::ParsingAtom)
    commitCurrentAtom();

  mlist().push_back(std::make_shared<math::StyleChange>(math::Style::SS));
}

} // namespace parsing

} // namespace tex

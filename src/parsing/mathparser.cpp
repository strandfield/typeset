// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/parsing/mathparser.h"

#include "tex/math/boundary.h"
#include "tex/math/fontchange.h"
#include "tex/math/fraction.h"
#include "tex/math/math-typeset.h"
#include "tex/math/mathlist.h"
#include "tex/math/stylechange.h"

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
  return std::make_shared<math::Atom>(type, nucleus(), subscript(), superscript(), nullptr, nullptr, math::Atom::NoLimits);
}

MathParser::MathParser()
{
  enter(State::ParsingMList);
}

const std::map<std::string, MathParser::CS>& MathParser::csmap()
{
  static const std::map<std::string, MathParser::CS> map = {
    {"left", CS::LEFT},
    {"right", CS::RIGHT},
    {"over", CS::OVER},
    {"sqrt", CS::SQRT},
    {"rm", CS::RM},
    {"textstyle", CS::TEXTSTYLE},
    {"scriptstyle", CS::SCRIPTSTYLE},
    {"scriptscriptstyle", CS::SCRIPTSCRIPTSTYLE},
  };

  return map;
}

MathParser::CS MathParser::cs(const std::string& name)
{
  auto it = csmap().find(name);

  if (it == csmap().end())
    throw std::runtime_error{ "Unknown control sequence" };

  return it->second;
}

void MathParser::writeControlSequence(CS csname)
{
  if (state() == State::ParsingAtom)
  {
    commitCurrentAtom();
  }

  switch (csname)
  {
  case CS::LEFT:
    return cs_left();
  case CS::RIGHT:
    return cs_right();
  case CS::OVER:
    return cs_over();
  case CS::SQRT:
    return cs_sqrt();
  case CS::RM:
    return cs_rm();
  case CS::TEXTSTYLE:
    return cs_textstyle();
  case CS::SCRIPTSTYLE:
    return cs_scriptstyle();
  case CS::SCRIPTSCRIPTSTYLE:
    return cs_scriptscriptstyle();
  }
}

MathList& MathParser::mlist()
{
  return m_lists.empty() ? m_mlist : *m_lists.back();
}

void MathParser::writeSymbol(const std::string& str)
{
  switch (state())
  {
  case State::ParsingMList:
    return parse_mlist(str);
  case State::ParsingAtom:
    return parse_atom(str);
  case State::ParsingNucleus:
  case State::ParsingSubscript:
  case State::ParsingSuperscript:
    return parse_mlist(str);
  case State::AwaitingSubscript:
  case State::AwaitingSuperscript:
    return parse_subsupscript(str);
  case State::ParsingBoundary:
    return parse_mlist(str);
  case State::ParsingLeft:
    return parse_left(str);
  case State::ParsingRight:
    return parse_right(str);
  case State::ParsingRad:
    return parse_rad(str);
  case State::ParsingFraction:
    return parse_mlist(str);
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
  if (state() == State::ParsingMList)
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
  if (state() == State::ParsingMList)
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
  if (state() == State::ParsingRad)
  {
    leaveState();

    enter(State::ParsingAtom);
    m_builders.emplace_back(AtomBuilder(math::Atom::Rad));
    enter(State::ParsingNucleus);
    m_builders.back().setNucleus(pushMathList());
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
  case State::ParsingFraction:
  {
    popList();
    leaveState();

    if (state() == State::ParsingNucleus || state() == State::ParsingSubscript || state() == State::ParsingSuperscript)
    {
      endMathList();
    }

    break;
  }
  default:
    throw std::runtime_error{ "Bad call to endMathList()" };
  }
}

void MathParser::finish()
{
  commitCurrentAtom();

  if (state() == State::ParsingFraction)
  {
    popList();
    leaveState();
  }

  assert(state() == State::ParsingMList);
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

void MathParser::mutate(State from, State to)
{
  leave(from);
  enter(to);
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

void MathParser::parse_mlist(const std::string& str)
{
  enter(State::ParsingAtom);
  m_builders.emplace_back();
  m_builders.back().setNucleus(std::make_shared<TextSymbol>(str));
}

void MathParser::parse_atom(const std::string& str)
{
  commitCurrentAtom();
  return writeSymbol(str);
}

void MathParser::parse_subsupscript(const std::string& str)
{
  if (state() == State::AwaitingSubscript)
  {
    m_builders.back().setSubscript(std::make_shared<TextSymbol>(str));
    leaveState();
  }
  else if (state() == State::AwaitingSuperscript)
  {
    m_builders.back().setSuperscript(std::make_shared<TextSymbol>(str));
    leaveState();
  }
}

void MathParser::parse_left(const std::string& str)
{
  mlist().push_back(std::make_shared<math::Boundary>(std::make_shared<TextSymbol>(str)));
  leave(State::ParsingLeft);
  assert(state() == State::ParsingBoundary);
}

void MathParser::parse_right(const std::string& str)
{
  mlist().push_back(std::make_shared<math::Boundary>(std::make_shared<TextSymbol>(str)));
  leave(State::ParsingRight);
  leave(State::ParsingBoundary);
  leave(State::ParsingNucleus);
  popList();
  assert(state() == State::ParsingAtom);
}

void MathParser::parse_rad(const std::string& str)
{
  leave(State::ParsingRad);

  m_builders.emplace_back(AtomBuilder(math::Atom::Rad).setNucleus(std::make_shared<TextSymbol>(str)));
  enter(State::ParsingAtom);
}

void MathParser::cs_left()
{
  m_builders.emplace_back(AtomBuilder(math::Atom::Inner).setNucleus(pushMathList()));
  enter(State::ParsingAtom);
  enter(State::ParsingNucleus);
  enter(State::ParsingBoundary);
  enter(State::ParsingLeft);
}

void MathParser::cs_right()
{
  if (state() != State::ParsingBoundary)
  {
    throw std::runtime_error{ "Mismatching \\left & \\right" };
  }

  enter(State::ParsingRight);
}

void MathParser::cs_over()
{
  MathList& ml = mlist();
  auto frac = std::make_shared<math::Fraction>(std::move(ml), MathList{});
  ml.clear();
  ml.push_back(frac);
  enter(State::ParsingFraction);
  pushList(frac->denom());
}

void MathParser::cs_sqrt()
{
  enter(State::ParsingRad);
}

void MathParser::cs_rm()
{
  mlist().push_back(std::make_shared<math::FontChange>(Font::MathRoman));
}

void MathParser::cs_textstyle()
{
  mlist().push_back(std::make_shared<math::StyleChange>(math::Style::T));
}

void MathParser::cs_scriptstyle()
{
  mlist().push_back(std::make_shared<math::StyleChange>(math::Style::S));
}

void MathParser::cs_scriptscriptstyle()
{
  mlist().push_back(std::make_shared<math::StyleChange>(math::Style::SS));
}

} // namespace parsing

} // namespace tex

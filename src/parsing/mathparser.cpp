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
  : m_type(t)
{
  if (m_type == math::Atom::Inner)
  {
    m_nucleus = std::make_shared<MathListNode>();
    m_state = ParsingNucleus;
  }
}

AtomBuilder::State AtomBuilder::state() const
{
  return m_state;
}

void AtomBuilder::setState(State s)
{
  m_state = s;
}

MathList& AtomBuilder::currentList()
{
  auto get_list = [](Node* n) -> MathList & {

    if (n == nullptr || !n->is<MathListNode>())
    {
      throw std::runtime_error{ "Could not get current math list" };
    }

    return n->as<MathListNode>().list();
  };

  switch (state())
  {
  case AtomBuilder::ParsingSubscript:
    return get_list(m_subscript.get());
  case AtomBuilder::ParsingSuperscript:
    return get_list(m_superscript.get());
  case AtomBuilder::ParsingNucleus:
  default:
    return get_list(m_nucleus.get());
  }
}

void AtomBuilder::setNucleus(const std::shared_ptr<Node>& node)
{
  m_nucleus = node;
}

void AtomBuilder::setSuperscript(const std::shared_ptr<Node>& node)
{
  assert(state() == AwaitingSuperscript);
  m_superscript = node;
}

void AtomBuilder::setSubscript(const std::shared_ptr<Node>& node)
{
  assert(state() == AwaitingSubscript);
  m_subscript = node;
}

std::shared_ptr<math::Atom> AtomBuilder::build() const
{
  //auto get_node = [](const std::shared_ptr<Node>& n) -> std::shared_ptr<Node> {

  //  if (n != nullptr && n->is<MathListNode>())
  //  {
  //    const MathList& mlist = n->as<MathListNode>().list();

  //    if (mlist.size() == 1 && mlist.front()->is<math::Fraction>())
  //    {
  //      return mlist.front();
  //    }
  //  }

  //  return n;
  //};

  return std::make_shared<math::Atom>(m_type, m_nucleus, m_subscript, m_superscript, nullptr, nullptr, math::Atom::NoLimits);
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
  switch (csname)
  {
  case CS::LEFT:
    return cs_left();
  case CS::RIGHT:
    return cs_right();
  case CS::OVER:
    return cs_over();
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
  auto get_mlist = [](MathList& mlist) -> MathList& {

    if (mlist.size() == 1 && mlist.front()->is<math::Fraction>())
    {
      return mlist.front()->as<math::Fraction>().denom();
    }

    return mlist;
  };

  if (m_builders.empty())
  {
    return get_mlist(m_mlist);
  }
  else
  {
    return get_mlist(m_builders.back().currentList());
  }
}

void MathParser::writeSymbol(const std::string& str)
{
  switch (state())
  {
  case State::ParsingMList:
    return parse_mlist(str);
  case State::ParsingBoundary:
    return parse_boundary(str);
  case State::ParsingLeft:
    return parse_left(str);
  case State::ParsingRight:
    return parse_right(str);
  }
}

void MathParser::writeBox(const std::shared_ptr<tex::Box>& box)
{
  throw std::runtime_error{ "Not implemented" };
}

void MathParser::beginSuperscript()
{
  if (m_builders.empty())
  {
    m_builders.emplace_back();
  }

  m_builders.back().setState(AtomBuilder::AwaitingSuperscript);
}

void MathParser::beginSubscript()
{
  if (m_builders.empty())
  {
    m_builders.emplace_back();
  }

  m_builders.back().setState(AtomBuilder::AwaitingSubscript);
}

void MathParser::beginMathList()
{
  if (m_builders.empty())
  {
    m_builders.emplace_back();
    m_builders.back().setNucleus(std::make_shared<MathListNode>());
    m_builders.back().setState(AtomBuilder::ParsingNucleus);
  }
  else
  {
    if (m_builders.back().state() == AtomBuilder::Idle)
    {
      auto atom = m_builders.back().build();
      m_builders.pop_back();
      mlist().push_back(atom);

      m_builders.emplace_back();
      m_builders.back().setNucleus(std::make_shared<MathListNode>());
      m_builders.back().setState(AtomBuilder::ParsingNucleus);

      return;
    }

    AtomBuilder& b = m_builders.back();

    if (b.state() == AtomBuilder::ParsingNucleus || b.state() == AtomBuilder::ParsingSubscript || b.state() == AtomBuilder::ParsingSuperscript)
    {
      m_builders.emplace_back();
      m_builders.back().setNucleus(std::make_shared<MathListNode>());
      m_builders.back().setState(AtomBuilder::ParsingNucleus);
    }
    else if (b.state() == AtomBuilder::AwaitingSuperscript)
    {
      m_builders.back().setSuperscript(std::make_shared<MathListNode>());
      m_builders.back().setState(AtomBuilder::ParsingSuperscript);
    }
    else if (b.state() == AtomBuilder::AwaitingSubscript)
    {
      m_builders.back().setSubscript(std::make_shared<MathListNode>());
      m_builders.back().setState(AtomBuilder::ParsingSubscript);
    }
  }
}

void MathParser::endMathList()
{
  commitCurrentAtom();

  assert(!m_builders.empty() && m_builders.back().state() != AtomBuilder::Idle);

  AtomBuilder& b = m_builders.back();

  if (b.state() == AtomBuilder::ParsingNucleus || b.state() == AtomBuilder::ParsingSubscript || b.state() == AtomBuilder::ParsingSuperscript)
  {
    m_builders.back().setState(AtomBuilder::Idle);
  }
  else
  {
    throw std::runtime_error{ "Could not end current math list" };
  }
}

void MathParser::finish()
{
  commitCurrentAtom();
}

MathList& MathParser::output()
{
  return m_mlist;
}

void MathParser::commitCurrentAtom()
{
  if (m_builders.empty())
    return;

  if (m_builders.back().state() == AtomBuilder::Idle)
  {
    auto atom = m_builders.back().build();
    m_builders.pop_back();
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
  m_states.pop_back();
}

void MathParser::parse_mlist(const std::string& str)
{
  if (m_builders.empty())
  {
    m_builders.emplace_back();
    m_builders.back().setNucleus(std::make_shared<TextSymbol>(str));
  }
  else
  {
    if (m_builders.back().state() == AtomBuilder::Idle)
    {
      auto atom = m_builders.back().build();
      m_builders.pop_back();
      mlist().push_back(atom);

      m_builders.emplace_back();
      m_builders.back().setNucleus(std::make_shared<TextSymbol>(str));

      return;
    }

    AtomBuilder& b = m_builders.back();

    if (b.state() == AtomBuilder::ParsingNucleus || b.state() == AtomBuilder::ParsingSubscript || b.state() == AtomBuilder::ParsingSuperscript)
    {
      m_builders.emplace_back();
      m_builders.back().setNucleus(std::make_shared<TextSymbol>(str));
    }
    else if (b.state() == AtomBuilder::AwaitingSuperscript)
    {
      b.setSuperscript(std::make_shared<TextSymbol>(str));
      b.setState(AtomBuilder::Idle);
    }
    else if (b.state() == AtomBuilder::AwaitingSubscript)
    {
      b.setSubscript(std::make_shared<TextSymbol>(str));
      b.setState(AtomBuilder::Idle);
    }
  }
}

void MathParser::parse_boundary(const std::string& str)
{
  return parse_mlist(str);
}

void MathParser::parse_left(const std::string& str)
{
  mlist().push_back(std::make_shared<math::Boundary>(std::make_shared<TextSymbol>(str)));
  leave(State::ParsingLeft);
}

void MathParser::parse_right(const std::string& str)
{
  commitCurrentAtom();
  mlist().push_back(std::make_shared<math::Boundary>(std::make_shared<TextSymbol>(str)));
  leave(State::ParsingRight);
  leave(State::ParsingBoundary);
  m_builders.back().setState(AtomBuilder::Idle);
}

void MathParser::cs_left()
{
  commitCurrentAtom();
  m_builders.emplace_back(math::Atom::Inner);
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
  commitCurrentAtom();

  MathList& ml = mlist();
  auto frac = std::make_shared<math::Fraction>(std::move(ml), MathList{});
  ml.clear();
  ml.push_back(frac);
}

void MathParser::cs_rm()
{
  commitCurrentAtom();
  mlist().push_back(std::make_shared<math::FontChange>(Font::MathRoman));
}

void MathParser::cs_textstyle()
{
  commitCurrentAtom();
  mlist().push_back(std::make_shared<math::StyleChange>(math::Style::T));
}

void MathParser::cs_scriptstyle()
{
  commitCurrentAtom();
  mlist().push_back(std::make_shared<math::StyleChange>(math::Style::S));
}

void MathParser::cs_scriptscriptstyle()
{
  commitCurrentAtom();
  mlist().push_back(std::make_shared<math::StyleChange>(math::Style::SS));
}

} // namespace parsing

} // namespace tex

// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/parsing/mathmode.h"

#include "tex/parsing/typesetting-machine.h"
#include "tex/parsing/horizontalmode.h"
#include "tex/parsing/verticalmode.h"

#include "tex/math/fontchange.h"
#include "tex/math/fraction.h"
#include "tex/math/math-typeset.h"
#include "tex/math/mathlist.h"
#include "tex/math/stylechange.h"

#include "tex/unicode.h"

#include <cassert>

namespace tex
{

namespace parsing
{

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

MathMode::MathMode(TypesettingMachine& m)
  : Mode(m),
  m_style(math::Style::D)
{
  if (parent() && dynamic_cast<HorizontalMode*>(parent()))
    m_style = math::Style::T;

  push(&MathMode::main_callback);

  m_commands = {
    {"over", &MathMode::cs_over},
    {"rm", &MathMode::cs_rm},
    {"textstyle", &MathMode::cs_textstyle},
    {"scriptstyle", &MathMode::cs_scriptstyle},
    {"scriptscriptstyle", &MathMode::cs_scriptscriptstyle},
  };

  read(tokens(), m_style == math::Style::T ? 1 : 2);
}

TypesettingMachine& MathMode::machine() const
{
  return static_cast<TypesettingMachine&>(Mode::machine());
}

FontMetrics MathMode::metrics() const
{
  const Options& opts = machine().options();
  return FontMetrics{ opts.font(), opts.fontSize(), machine().typesetEngine()->metrics() };
}

bool MathMode::isInlineMath() const
{
  return m_style == math::Style::T;
}

std::map<std::string, MathMode::Callback>& MathMode::commands()
{
  return m_commands;
}

void MathMode::push(Callback cmd)
{
  m_callbacks.push_back(cmd);
}

void MathMode::pop()
{
  m_callbacks.pop_back();
}

RetCode MathMode::advance()
{
  return m_callbacks.back()(*this);
}

std::string& MathMode::symbuf()
{
  return m_symbuf;
}

MathList& MathMode::mlist()
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
    return get_mlist(m_hlist);
  }
  else
  {
    return get_mlist(m_builders.back().currentList());
  }
}

void MathMode::writeSymbol(const std::string& str)
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
    else if(b.state() == AtomBuilder::AwaitingSubscript)
    {
      b.setSubscript(std::make_shared<TextSymbol>(str));
      b.setState(AtomBuilder::Idle);
    }
  }
}

void MathMode::beginSuperscript()
{
  if (m_builders.empty())
  {
    m_builders.emplace_back();
  }

  m_builders.back().setState(AtomBuilder::AwaitingSuperscript);
}

void MathMode::beginSubscript()
{
  if (m_builders.empty())
  {
    m_builders.emplace_back();
  }

  m_builders.back().setState(AtomBuilder::AwaitingSubscript);
}

void MathMode::beginMathList()
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

void MathMode::endMathList()
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

void MathMode::commitCurrentAtom()
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

void MathMode::writeOutput()
{
  VerticalMode* vm = dynamic_cast<VerticalMode*>(parent());

  if (vm)
  {
    Options opts = machine().options().withStyle(math::Style::D)
      .withFont(Font::MathItalic);
    List hlist = tex::mlist_to_hlist(std::move(m_hlist), opts);

    std::shared_ptr<HBox> box = tex::hbox(std::move(hlist));

    vm->vlist().insert(vm->vlist().end(), box);
  }
  else
  {
    HorizontalMode* hm = dynamic_cast<HorizontalMode*>(parent());

    Options opts = machine().options().withStyle(math::Style::T)
      .withFont(Font::MathItalic);
    List hlist = tex::mlist_to_hlist(std::move(m_hlist), opts);

    hm->hlist().insert(hm->hlist().end(), hlist.begin(), hlist.end());
  }

  m_hlist.clear();
}

RetCode MathMode::main_callback(MathMode& self)
{
  std::vector<Token>& tokens = self.tokens();

  if (peek(tokens).isControlSequence())
  {
    auto it = self.commands().find(tokens.front().controlSequence());

    if (it == self.commands().end())
    {
      throw std::runtime_error{ "No such control sequence in Horizontal mode" };
    }

    return it->second(self);
  }
  else
  {
    if (peek(tokens) == CharCategory::MathShift)
    {
      if (self.isInlineMath())
      {
        read(tokens);
        self.commitCurrentAtom();
        self.writeOutput();
        return RetCode::Return;
      }

      if (tokens.size() == 1)
      {
        return RetCode::Await; // need more tokens
      }

      if (peek(tokens, 1) == CharCategory::MathShift)
      {
        read(tokens, 2);
        self.commitCurrentAtom();
        self.writeOutput();
        return RetCode::Return;
      }
      else
      {
        read(tokens);
        throw std::runtime_error{ "Unexpected '$' in display math mode" };
      }

      return RetCode::Yield;
    }

    CharacterToken ctok = read(tokens).characterToken();

    if (ctok.category == CharCategory::Letter || ctok.category == CharCategory::Other)
    {
      self.symbuf().push_back(ctok.value);

      if (is_utf8_char(self.symbuf()))
      {
        self.writeSymbol(self.symbuf());
        self.symbuf().clear();
      }
    }
    else if (ctok.category == CharCategory::Subscript)
    {
      self.beginSubscript();
    }
    else if (ctok.category == CharCategory::Superscript)
    {
      self.beginSuperscript();
    }
    else if (ctok.category == CharCategory::GroupBegin)
    {
      self.beginMathList();
    }
    else if (ctok.category == CharCategory::GroupEnd)
    {
      self.endMathList();
    }
   
    return RetCode::Yield;
  }
}

RetCode MathMode::cs_over(MathMode& self)
{
  self.commitCurrentAtom();
  read(self.tokens(), 1);

  MathList& mlist = self.mlist();
  auto frac = std::make_shared<math::Fraction>(std::move(mlist), MathList{});
  mlist.clear();
  mlist.push_back(frac);

  return RetCode::Yield;
}

RetCode MathMode::cs_rm(MathMode& self)
{
  self.commitCurrentAtom();
  read(self.tokens(), 1);
  self.mlist().push_back(std::make_shared<math::FontChange>(Font::MathRoman));
  return RetCode::Yield;
}

RetCode MathMode::cs_textstyle(MathMode& self)
{
  self.commitCurrentAtom();
  read(self.tokens(), 1);
  self.mlist().push_back(std::make_shared<math::StyleChange>(math::Style::T));
  return RetCode::Yield;
}

RetCode MathMode::cs_scriptstyle(MathMode& self)
{
  self.commitCurrentAtom();
  read(self.tokens(), 1);
  self.mlist().push_back(std::make_shared<math::StyleChange>(math::Style::S));
  return RetCode::Yield;
}

RetCode MathMode::cs_scriptscriptstyle(MathMode& self)
{
  self.commitCurrentAtom();
  read(self.tokens(), 1);
  self.mlist().push_back(std::make_shared<math::StyleChange>(math::Style::SS));
  return RetCode::Yield;
}

} // namespace parsing

} // namespace tex

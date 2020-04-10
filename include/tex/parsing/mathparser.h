// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_PARSING_MATHPARSER_H
#define LIBTYPESET_PARSING_MATHPARSER_H

#include "tex/math/atom.h"
#include "tex/math/mathlist.h"
#include "tex/math/style.h"

#include "tex/fontmetrics.h"
#include "tex/mathcode.h"
#include "tex/hbox.h"

#include <map>
#include <vector>

namespace tex
{

namespace parsing
{

class LIBTYPESET_API AtomBuilder
{
public:
  math::Atom::Type type = math::Atom::Ord;
  math::Atom::LimitsFlag limits = math::Atom::NoLimits;
  std::shared_ptr<Node> nucleus_;
  std::shared_ptr<Node> superscript_;
  std::shared_ptr<Node> subscript_;

public:
  AtomBuilder();
  explicit AtomBuilder(math::Atom::Type t);

  const std::shared_ptr<Node>& nucleus() const { return nucleus_; }
  const std::shared_ptr<Node>& superscript() const { return superscript_; }
  const std::shared_ptr<Node>& subscript() const { return subscript_; }

  AtomBuilder& setNucleus(const std::shared_ptr<Node>& node);
  AtomBuilder& setSuperscript(const std::shared_ptr<Node>& node);
  AtomBuilder& setSubscript(const std::shared_ptr<Node>& node);

  std::shared_ptr<math::Atom> build() const;
};

class LIBTYPESET_API MathParser
{
public:
  MathParser();

  enum class State
  {
    ParsingMList,
    /* Atom */
    ParsingAtom,
    ParsingNucleus,
    AwaitingSuperscript,
    ParsingSuperscript,
    AwaitingSubscript,
    ParsingSubscript,
    /* Boundary */
    ParsingBoundary,
    ParsingLeft,
    ParsingRight,
    /* Sqrt */
    ParsingSqrt,
    ParsingSqrtDegree,
    ParsingSqrtRadicand,
    ParsingSqrtRadicandMList,
    /* Fraction */
    ParsingOver,
    ParsingFrac,
    ParsingFracNumer,
    ParsingFracNumerMList,
    ParsingFracDenom,
    ParsingFracDenomMList,
  };

  State state() const;
  const std::vector<State>& states() const;

  void writeSymbol(std::shared_ptr<MathSymbol> mathsym);

  void writeBox(const std::shared_ptr<tex::Box>& box);

  void beginSuperscript();
  void beginSubscript();

  void beginMathList();
  void endMathList();

  /* Control sequences */
  void left();
  void right();
  void over();
  void frac();
  void sqrt();
  void textstyle();
  void scriptstyle();
  void scriptscriptstyle();

  void finish();

  MathList& output();

protected:
  MathList& mlist();
  void commitCurrentAtom();

  void enter(State s);
  void leave(State s);
  void leaveState();

  void pushList(MathList& l);
  void popList();
  std::shared_ptr<MathListNode> pushMathList();

  bool isParsingMList() const;

  /* Parsing procedures */
  void parse_mlist(std::shared_ptr<MathSymbol> mathsym);

  void parse_atom(std::shared_ptr<MathSymbol> mathsym);
  void parse_subsupscript(std::shared_ptr<MathSymbol> mathsym);

  void parse_left(std::shared_ptr<MathSymbol> mathsym);
  void parse_right(std::shared_ptr<MathSymbol> mathsym);

  void parse_sqrt(std::shared_ptr<MathSymbol> mathsym);
  void parse_sqrt_degree(std::shared_ptr<MathSymbol> mathsym);
  void parse_sqrt_radicand(std::shared_ptr<MathSymbol> mathsym);

  void parse_frac_numer(std::shared_ptr<MathSymbol> mathsym);
  void parse_frac_denom(std::shared_ptr<MathSymbol> mathsym);

private:
  std::vector<State> m_states;
  std::vector<MathList*> m_lists;
  MathList m_mlist;
  std::vector<AtomBuilder> m_builders;
};

} // namespace parsing

} // namespace tex

namespace tex
{
namespace parsing
{

inline MathParser::State MathParser::state() const
{
  return m_states.back();
}

inline const std::vector<MathParser::State>& MathParser::states() const
{
  return m_states;
}

} // namespace parsing
} // namespace tex

#endif // LIBTYPESET_PARSING_MATHPARSER_H

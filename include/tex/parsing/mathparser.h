// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_PARSING_MATHPARSER_H
#define LIBTYPESET_PARSING_MATHPARSER_H

#include "tex/math/atom.h"
#include "tex/math/mathlist.h"
#include "tex/math/style.h"

#include "tex/fontmetrics.h"
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
  enum State
  {
    Idle,
    ParsingNucleus,
    AwaitingSuperscript,
    ParsingSuperscript,
    AwaitingSubscript,
    ParsingSubscript,
  };

  AtomBuilder();
  explicit AtomBuilder(math::Atom::Type t);

  State state() const;
  void setState(State s);

  MathList& currentList();

  const std::shared_ptr<Node>& nucleus() { return m_nucleus; }
  const std::shared_ptr<Node>& superscript() { return m_superscript; }
  const std::shared_ptr<Node>& subscript() { return m_subscript; }

  void setNucleus(const std::shared_ptr<Node>& node);
  void setSuperscript(const std::shared_ptr<Node>& node);
  void setSubscript(const std::shared_ptr<Node>& node);

  std::shared_ptr<math::Atom> build() const;
  
private:
  State m_state = Idle;
  math::Atom::Type m_type = math::Atom::Ord;
  std::shared_ptr<Node> m_nucleus;
  std::shared_ptr<Node> m_superscript;
  std::shared_ptr<Node> m_subscript;
};

class LIBTYPESET_API MathParser
{
public:
  MathParser();

  enum class State
  {
    ParsingMList,
    ParsingBoundary,
    ParsingLeft,
    ParsingRight,
  };

  State state() const;
  const std::vector<State>& states() const;

  enum class CS
  {
    LEFT,
    RIGHT,
    OVER,
    RM,
    TEXTSTYLE,
    SCRIPTSTYLE,
    SCRIPTSCRIPTSTYLE,
  };

  static const std::map<std::string, CS>& csmap();
  static CS cs(const std::string& name);

  void writeControlSequence(CS csname);

  void writeSymbol(const std::string& str);  
  void writeBox(const std::shared_ptr<tex::Box>& box);

  void beginSuperscript();
  void beginSubscript();

  void beginMathList();
  void endMathList();

  void finish();

  MathList& output();

protected:
  MathList& mlist();
  void commitCurrentAtom();

  void enter(State s);
  void leave(State s);

  /* Parsing procedures */
  void parse_mlist(const std::string& str);

  void parse_boundary(const std::string& str);

  void parse_left(const std::string& str);
  void parse_right(const std::string& str);

  /* Control sequences */
  void cs_left();
  void cs_right();
  void cs_over();
  void cs_rm();
  void cs_textstyle();
  void cs_scriptstyle();
  void cs_scriptscriptstyle();

private:
  std::vector<State> m_states;
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

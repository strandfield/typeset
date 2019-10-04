// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_PARSING_MATHMODE_H
#define LIBTYPESET_PARSING_MATHMODE_H

#include "tex/parsing/mode.h"

#include "tex/parsing/typesetting-machine.h"

#include "tex/math/atom.h"
#include "tex/math/mathlist.h"
#include "tex/math/style.h"

#include "tex/fontmetrics.h"
#include "tex/hbox.h"
#include "tex/typeset.h"

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

class LIBTYPESET_API MathMode : public Mode
{
public:
  MathMode(TypesettingMachine& m);
  ~MathMode() = default;

  typedef RetCode(*Callback)(MathMode&);

  TypesettingMachine& machine() const;
  FontMetrics metrics() const;

  bool isInlineMath() const;

  std::map<std::string, Callback>& commands();
  void push(Callback cmd);
  void pop();

  RetCode advance() override;

  std::string& symbuf();
  void writeSymbol(const std::string& str);

  void beginSuperscript();
  void beginSubscript();

  void beginMathList();
  void endMathList();

  void commitCurrentAtom();

  MathList& mlist();

  void writeOutput();

  /* Callbacks */
  static RetCode main_callback(MathMode&);

  /* Control sequences */
  static RetCode cs_over(MathMode&);
  static RetCode cs_rm(MathMode&);
  static RetCode cs_textstyle(MathMode&);
  static RetCode cs_scriptstyle(MathMode&);
  static RetCode cs_scriptscriptstyle(MathMode&);

private:
  math::Style m_style;
  std::vector<Callback> m_callbacks;
  std::map<std::string, Callback> m_commands;
  std::string m_symbuf;
  std::vector<AtomBuilder> m_builders;
  MathList m_hlist;
};

} // namespace parsing

} // namespace tex

#endif // LIBTYPESET_PARSING_MATHMODE_H

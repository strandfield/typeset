// Copyright (C) 2019-2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_MATH_TYPESET_H
#define LIBTYPESET_MATH_TYPESET_H

#include "tex/typeset.h"
#include "tex/math/mathlist.h"
#include "tex/math/style.h"

namespace tex
{

namespace math
{
class Atom;
} // namespace math

class HBox;
class VBox;

struct MathFont
{
  Font textfont;
  Font scriptfont;
  Font scriptscriptfont;
};

class LIBTYPESET_API MathTypesetter
{
private:
  std::shared_ptr<TypesetEngine> m_engine;
  std::array<MathFont, 16> m_fonts;
  int m_relpenalty = 500;
  int m_binoppenalty = 700;
  bool m_insert_penalties = true;
  math::Style m_current_style = math::Style::D;
  std::shared_ptr<math::Atom> m_most_recent_atom;

public:
  explicit MathTypesetter(std::shared_ptr<TypesetEngine> engine);

  TypesetEngine& engine() const;
  std::shared_ptr<TypesetEngine> sharedEngine() const;

  void setFonts(const std::array<MathFont, 16>& fonts);

  int relpenalty() const;
  int binoppenalty() const;

  void setRelpenalty(int p);
  void setBinoppenalty(int p);

  bool insertPenalties() const;
  void setInsertPenalties(bool on = true);

  List mlist2hlist(MathList mlist, math::Style style = math::Style::D);

private:

  static constexpr int SigmaFamily = 2;
  static constexpr int XiFamily = 3;

  Font getFont(int fam) const;
  Font getFont(int fam, math::Style style) const;
  FontMetrics getMetrics(int fam) const;
  FontMetrics getMetrics(int fam, math::Style style) const;

  template<size_t I>
  float sigma() const;

  template<size_t I>
  float xi() const;

  template<size_t I>
  float sigma(math::Style style) const;

  static std::shared_ptr<Box> nullbox();
  
  std::shared_ptr<Box> typeset(std::shared_ptr<MathSymbol> symbol);
  std::shared_ptr<Box> typesetDelimiter(const std::shared_ptr<Symbol>& ms, float minTotalHeight);
  std::shared_ptr<VBox> radicalSignBox(float minTotalHeight);
  std::shared_ptr<Box> boxit(std::shared_ptr<Node> node);
  std::shared_ptr<Box> boxit(std::shared_ptr<Node> node, math::Style s);
  std::shared_ptr<HBox> boxit(MathList mlist);
  std::shared_ptr<HBox> boxit(MathList mlist, math::Style s);
  std::shared_ptr<HBox> hboxit(std::shared_ptr<Node> node);

  void preprocess(MathList& mlist);

  void processOpAtom(MathList& mlist, MathList::iterator& current);
  void processRadAtom(MathList& mathlist, MathList::iterator& current);
  void processAccAtom(MathList& mlist, MathList::iterator& current);
  void changeToOrd(MathList& mlist, MathList::iterator& current);
  void processAtom(MathList& mlist, MathList::iterator& current);
  bool isCharacterBox(const std::shared_ptr<Node>& node, float* w = nullptr, float* h = nullptr, float* d = nullptr);
  void attachSubSup(MathList& mlist, MathList::iterator& current);
  void processFraction(MathList& mlist, MathList::iterator& current);
  void processRoot(MathList& mlist, MathList::iterator& current);
  void processBoundary(MathList& mlist);

  void insertSpace(List& list, const std::shared_ptr<math::Atom>& preceding, const std::shared_ptr<math::Atom>& next);
  std::shared_ptr<Glue> thinmuskip();
  std::shared_ptr<Glue> medmuskip();
  std::shared_ptr<Glue> thickmuskip();
};

} // namespace tex

#endif // LIBTYPESET_MATH_TYPESET_H

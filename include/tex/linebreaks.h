// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_LINEBREAKS_H
#define LIBTYPESET_LINEBREAKS_H

#include "tex/hbox.h"

#include <vector>

namespace tex
{

enum class FitnessClass {
  Tight = 0,
  Decent = 1,
  Loose = 2,
  VeryLoose = 3,
};

class LIBTYPESET_API Paragraph final
{
public:
  int tolerance = /* 200 */ 800;
  int adjdemerits = 10'000;
  int linepenalty = 10;
  float hsize = 800.f;
  std::vector<float> parshape;
  std::shared_ptr<Glue> leftskip;
  std::shared_ptr<Glue> rightskip;
  std::shared_ptr<Glue> parfillskip;
  std::shared_ptr<Glue> baselineskip;
  std::shared_ptr<Glue> lineskip;
  float lineskiplimit;
  float prevdepth = -10000.f;

public:
  Paragraph();

  using Badness = int;
  using Demerits = int;

  float linelength(size_t n) const;

  struct Totals
  {
    Totals();

    float width;
    GlueStretch stretch;
    GlueShrink shrink;
  };

  struct Breakpoint 
  {
    List::const_iterator position;
    Demerits demerits;
    size_t line;
    FitnessClass fitness;
    Totals totals;
    std::shared_ptr<Breakpoint> previous;

    Breakpoint(const List::const_iterator & pos, std::shared_ptr<Breakpoint> prev = nullptr);
    Breakpoint(const List::const_iterator & pos, Demerits d, size_t l, FitnessClass fc, Totals t, std::shared_ptr<Breakpoint> prev);
  };

  std::vector<Breakpoint> computeBreakpoints(const List & hlist);

  void prepare(List & hlist);
  List create(const List & hlist);

  static Badness computeBadness(float glueSetRatio);
  static FitnessClass getFitnessClass(float glueSetRatio);
  static FitnessClass getFitnessClass(float glueSetRatio, Badness b);

  static bool checkCompatibility(FitnessClass a, FitnessClass b);

  static Demerits computeDemerits(int lpenalty, Badness b, int penal);

protected:
  /// Linebreaking
  float computeGlueRatio(const Totals & sum, Breakpoint & active, size_t current_line);
  Totals computeTotals(const List & hlist, const Totals & sum, List::const_iterator breakpointpos);
  void tryBreak(std::list<std::shared_ptr<Breakpoint>> & activeBreakpoints, const List &hlist, List::const_iterator it, const Totals & sum);

  /// Paragraph creation
  std::shared_ptr<HBox> createLine(size_t linenum, List::const_iterator begin, List::const_iterator end);

protected:
  static bool isDiscardable(const Node & node);
  static bool isForcedLinebreak(const Node & node);
  static bool isForbiddenLinebreak(const Node & node);
  static void consumeDiscardable(List::const_iterator & it);
};

} // namespace tex

#endif // LIBTYPESET_LINEBREAKS_H

// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/linebreaks.h"

#include "tex/glue.h"
#include "tex/kern.h"
#include "tex/penalty.h"
#include "tex/vbox.h"

#include <algorithm>
#include <cmath>
#include <numeric>
#include <stdexcept>
#include <utility>

namespace tex
{

Paragraph::Totals::Totals()
  : width(0.f)
  , stretch(0.f)
  , shrink(0.f)
{

}

Paragraph::Breakpoint::Breakpoint(const List::const_iterator & pos, std::shared_ptr<Breakpoint> prev)
  : position(pos)
  , demerits(0)
  , line(0)
  , fitness(FitnessClass::Tight)
  , totals()
  , previous(prev)
{

}

Paragraph::Breakpoint::Breakpoint(const List::const_iterator & pos, Demerits d, size_t l, FitnessClass fc, Totals t, std::shared_ptr<Breakpoint> prev)
  : position(pos)
  , demerits(d)
  , line(l)
  , fitness(fc)
  , totals(t)
  , previous(prev)
{

}

Paragraph::Paragraph()
{
  leftskip = std::make_shared<Glue>(0.f, 0.f, 0.f);
  rightskip = leftskip;
  baselineskip = std::make_shared<Glue>(12.f, 0.f, 2.f);
  lineskip = std::make_shared<Glue>(3.f, -1.f, 0.f);
  lineskiplimit = 2.f;
  parfillskip = std::make_shared<Glue>(0.f, 0.f, 1.f, GlueOrder::Normal, GlueOrder::Fil);
}

float Paragraph::linelength(size_t n) const
{
  if (!parshape.empty())
  {
    if(n >= parshape.size())
      return parshape.back().length;
    else
      return parshape.at(n).length;
  }

  return hsize;
}

std::vector<Paragraph::Breakpoint> Paragraph::computeBreakpoints(const List & hlist)
{
  std::list<std::shared_ptr<Breakpoint>> activeNodes;
  Totals sum;
  List::const_iterator prevnode = hlist.end();

  activeNodes.push_back(std::make_shared<Breakpoint>(hlist.begin(), 0, 0, FitnessClass::Tight, Totals{}, nullptr));

  for (auto it = hlist.begin(); it != hlist.end(); ++it)
  {
    std::shared_ptr<Node> node = *it;

    if (node->isBox())
    {
      sum.width += node->as<Box>().width();
    }
    else if (node->isGlue())
    {
      if (prevnode != hlist.end() && (*prevnode)->isBox())
        tryBreak(activeNodes, hlist, it, sum);

      Glue & g = node->as<Glue>();
      sum.width += g.space();
      g.accumulate(sum.shrink, sum.stretch);
    }
    else if (node->isKern())
    {
      sum.width += node->as<Kern>().space();
    }
    else if (node->isPenalty() && !isForbiddenLinebreak(*node))
    {
      tryBreak(activeNodes, hlist, it, sum);
    }

    prevnode = it;
  }

  if (activeNodes.size() == 0) 
    throw std::runtime_error{ "Failed" };
  
  auto best_breakpoint = *activeNodes.begin();
  for (auto it = activeNodes.begin(); it != activeNodes.end(); ++it)
  {
    if ((*it)->demerits < best_breakpoint->demerits)
      best_breakpoint = *it;
  }

  std::vector<Breakpoint> result;
  do
  {
    result.push_back(*best_breakpoint);
    best_breakpoint = best_breakpoint->previous;
  } while (best_breakpoint != nullptr);

  std::reverse(result.begin(), result.end());

  return result;
}

void Paragraph::prepare(List & hlist)
{
  if (hlist.empty())
    return;

  if (hlist.back()->isGlue())
    hlist.pop_back();

  hlist.push_back(infinitePenalty());
  hlist.push_back(parfillskip);
  hlist.push_back(penalty(-Penalty::Infinity));
}

List Paragraph::create(const List & hlist)
{
  if (hlist.empty())
    return hlist;

  std::vector<Breakpoint> breakpoints = computeBreakpoints(hlist);

  List::const_iterator it = hlist.begin();
  List::const_iterator next = std::next(it);

  auto bp = std::next(breakpoints.begin());

  List result;

  while (bp != breakpoints.end())
  {
    auto line = createLine(bp->line, it, bp->position);

    VListBuilder::push_back(result, line, prevdepth, baselineskip, lineskip, lineskiplimit);

    it = bp->position;
    ++bp;

    if(bp != breakpoints.end())
      consumeDiscardable(it);
  }

  return result;
}

Paragraph::Badness Paragraph::computeBadness(float glueSetRatio)
{
  return std::min((int)(100 * std::pow(std::abs(glueSetRatio), 3)), 10'000);
}

FitnessClass Paragraph::getFitnessClass(float glueSetRatio)
{
  if (glueSetRatio < -0.5)
    return FitnessClass::Tight;
  else if (glueSetRatio <= 0.5)
    return FitnessClass::Decent;
  else if (glueSetRatio <= 1)
    return FitnessClass::Loose;
  else
    return FitnessClass::VeryLoose;
}

FitnessClass Paragraph::getFitnessClass(float glueSetRatio, Badness b)
{
  if (b >= 13)
  {
    if (glueSetRatio < 0.f)
      return FitnessClass::Tight;
    else if (b < 100)
      return FitnessClass::Loose;
    return FitnessClass::VeryLoose;
  }

  return FitnessClass::Decent;
}

bool Paragraph::checkCompatibility(FitnessClass a, FitnessClass b)
{
  return std::abs(static_cast<int>(a) - static_cast<int>(b)) <= 1;
}

Paragraph::Demerits Paragraph::computeDemerits(int l, Badness b, int p)
{
  if (0 <= p && p < 10'000)
    return Demerits(std::pow(l + b, 2) + std::pow(p, 2));
  else if (-10'000 < p && p < 0)
    return Demerits(std::pow(l + b, 2) - std::pow(p, 2));
  else
    return Demerits(std::pow(l + b, 2));
}

float Paragraph::computeGlueRatio(const Totals & sum, Breakpoint & active, size_t current_line)
{
  float width = sum.width - active.totals.width;

  float line_length = linelength(current_line - 1);

  if (width < line_length)
  {
    auto diff = sum.stretch - active.totals.stretch;
    if (diff.order() != GlueOrder::Normal)
      return 0.f;

    const float stretch = diff.normal;
    if (stretch > 0.f)
      return (line_length - width) / stretch;
    else
      return (float) Penalty::Infinity;
  }
  else if (width > line_length)
  {
    auto diff = sum.shrink - active.totals.shrink;
    if (diff.order() != GlueOrder::Normal)
      return 0;

    const float shrink = diff.normal;
    if (shrink > 0.f)
      return (line_length - width) / shrink;
    else
      return (float) Penalty::Infinity;
  }
  
  return 0.f;
}

Paragraph::Totals Paragraph::squeezeDiscardables(Totals sum, List::const_iterator breakpointpos, List::const_iterator end)
{
  /// Computes totals from breakpoint up to next box or forced linebreak
  for (auto it = breakpointpos; it != end; ++it)
  {
    std::shared_ptr<Node> node = *it;

    if (node->is<Glue>())
    {
      Glue & g = node->as<Glue>();
      sum.width += g.space();
      g.accumulate(sum.shrink, sum.stretch);
    }
    else if (node->isKern())
    {
      sum.width += node->as<Kern>().space();
    }
    else if (node->isBox() || (it != breakpointpos && isForcedLinebreak(*node)))
    {
      break;
    }
  }

  return sum;
}

struct Candidate
{
  std::shared_ptr<Paragraph::Breakpoint> active;
  Paragraph::Demerits demerits;
};

/*!
 * \fn void tryBreak(std::list<std::shared_ptr<Breakpoint>> & activeBreakpoints, const List &hlist, List::const_iterator it, const Totals & sum)
 * \param list of active breakpoints
 * \param list of all nodes
 * \param place to attempt a breakpoint
 * \param cumulated space and glue from the beginning of the list up to 'it'
 * \brief Compute new possible breakpoints
 *
 * This procedure attempts to create new possible breakpoints at the given position \a it.
 * For each breakpoint \c b in the list of active breakpoints, the procedure checks if the line 
 * formed of the nodes in \c{[b, it)} has an acceptable badness. 
 * If that is the case, a new active breakpoint is created.
 *
 * This procedure also removes active breakpoints if they are too far from the current position.
 *
 * This procedure is called for every node in the hlist.
 * When all nodes have been processed, the list of active breakpoints only contains 
 * final breakpoints of a paragraph.
 *
 */
void Paragraph::tryBreak(std::list<std::shared_ptr<Breakpoint>> & activeBreakpoints, const List &hlist, List::const_iterator it, Totals sum)
{
  auto active = activeBreakpoints.begin();
  size_t current_line = 0;
  const float maxratio = std::pow(tolerance / 100.f, 1.f / 3.f);

  const std::shared_ptr<Node> node = *it;

  while (active != activeBreakpoints.end())
  {
    Candidate candidates[4] = {
      Candidate{ nullptr, std::numeric_limits<int>::max() },
      Candidate{ nullptr, std::numeric_limits<int>::max() },
      Candidate{ nullptr, std::numeric_limits<int>::max() },
      Candidate{ nullptr, std::numeric_limits<int>::max() },
    };

    while (active != activeBreakpoints.end())
    {
      auto next = std::next(active);
      current_line = (*active)->line;
      float ratio = computeGlueRatio(sum, **active, current_line);
      std::shared_ptr<Breakpoint> active_bp = *active;


      // Deactivate breakpoints if they are too far from the current node.
      if (ratio < -1 || isForcedLinebreak(*node))
        activeBreakpoints.erase(active);

      if (-1 <= ratio && ratio <= maxratio)
      {
        Badness badness = computeBadness(ratio);

        Demerits d = computeDemerits(linepenalty, badness, node->isPenalty() ? node->as<Penalty>().value() : 0);

        FitnessClass fc = getFitnessClass(ratio);

        if (!checkCompatibility(fc, active_bp->fitness))
          d += adjdemerits;

        d += active_bp->demerits;

        if (d < candidates[static_cast<int>(fc)].demerits)
          candidates[static_cast<int>(fc)] = Candidate{ active_bp, d };
      }

      active = next;

      // With this condition, active breakpoints will be sorted by line number.
      if (active != activeBreakpoints.end() && (*active)->line >= current_line)
        break;
    }

    // Adds the discarded nodes to the current breakpoint
    Totals local_sum = squeezeDiscardables(sum, it, hlist.cend());

    for (size_t i = 0; i < 4; ++i) 
    {
      FitnessClass current_fc = static_cast<FitnessClass>(i);
      Candidate c = candidates[i];

      if (c.demerits < std::numeric_limits<int>::max()) 
      {
        auto bp = std::make_shared<Breakpoint>(it, c.demerits, c.active->line + 1, current_fc, local_sum, c.active);
        if (active != activeBreakpoints.end())
          activeBreakpoints.insert(active, bp);
        else
          activeBreakpoints.push_back(bp);
      }
    }
  }
}

std::shared_ptr<HBox> Paragraph::createLine(size_t linenum, List::const_iterator begin, List::const_iterator end)
{
  List hlist;
  hlist.push_back(leftskip);
  hlist.insert(hlist.end(), begin, end);
  hlist.push_back(rightskip);

  return hbox(std::move(hlist), linelength(linenum));
}

bool Paragraph::isDiscardable(const Node & node)
{
  return node.isKern() || node.isGlue() || node.isPenalty();
}

bool Paragraph::isForcedLinebreak(const Node & node)
{
  return node.isPenalty() && node.as<Penalty>().value() <= -Penalty::Infinity;
}

bool Paragraph::isForbiddenLinebreak(const Node & node)
{
  return node.isPenalty() && node.as<Penalty>().value() >= Penalty::Infinity;
}

void Paragraph::consumeDiscardable(List::const_iterator & it)
{
  while (isDiscardable(**it))
    ++it;
}

} // namespace tex

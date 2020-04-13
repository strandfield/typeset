// Copyright (C) 2019-2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/math/math-typeset.h"

#include "tex/glue.h"
#include "tex/hbox.h"
#include "tex/kern.h"
#include "tex/penalty.h"
#include "tex/rule.h"
#include "tex/vbox.h"
#include "tex/math/atom.h"
#include "tex/math/boundary.h"
#include "tex/math/fontchange.h"
#include "tex/math/fraction.h"
#include "tex/math/root.h"
#include "tex/math/style.h"
#include "tex/math/stylechange.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <stdexcept>
#include <set>

namespace tex
{

namespace math
{

namespace SpacingTable
{

static const int spacing[][8] = {
  { 0, 1, 2, 3, 0, 0, 0, 1 },
  { 1, 1, -1, 3, 0, 0, 0, 1 },
  { 2, 2, -1, -1, 2, -1, -1, 2 },
  { 3, 3, -1, 0, 3, 0, 0, 3 },
  { 0, 0, -1, 0, 0, 0, 0, 0 },
  { 0, 1, 2, 3, 0, 0, 0, 1 },
  { 1, 1, -1, 1, 1, 1, 1, 1 },
  { 1, 1, 2, 3, 1, 0, 1, 1 },
};

static const bool spacingNonScript[][8] = {
  { false, false, true, true, false, false, false, true },
  { false, false, false, true, false, false, false, true },
  { true, true, false, false, true, false, false, true },
  { true, true, false, false, true, false, false, true },
  { false, false, false, false, false, false, false, false },
  { false, false, true, true, false, false, false, true },
  { true, true, false, true, true, true, true, true },
  { true, false, true, true, true, false, true, true },
};

} // namespace SpacingTable

} // namespace math


template<size_t I>
float MathTypesetter::sigma() const
{
  return sigma<I>(m_current_style);
}

template<size_t I>
float MathTypesetter::sigma(math::Style style) const
{
  return getMetrics(SigmaFamily, style).sigma<I>();
}

template<size_t I>
float MathTypesetter::xi() const
{
  return getMetrics(XiFamily).xi<I>();
}

struct RAIIStyleGuard
{
  math::Style& style;
  math::Style saved_value;

  RAIIStyleGuard(math::Style& s)
    : style(s),
      saved_value(s)
  {

  }

  ~RAIIStyleGuard()
  {
    style = saved_value;
  }
};


MathTypesetter::MathTypesetter(std::shared_ptr<TypesetEngine> engine)
  : m_engine(std::move(engine)),
    m_fonts{}
{

}

TypesetEngine& MathTypesetter::engine() const
{
  return *m_engine;
}

std::shared_ptr<TypesetEngine> MathTypesetter::sharedEngine() const
{
  return m_engine;
}

void MathTypesetter::setFonts(const std::array<MathFont, 16>& fonts)
{
  m_fonts = fonts;
}

int MathTypesetter::relpenalty() const
{
  return m_relpenalty;
}

int MathTypesetter::binoppenalty() const
{
  return m_binoppenalty;
}

void MathTypesetter::setRelpenalty(int p)
{
  m_relpenalty = p;
}

void MathTypesetter::setBinoppenalty(int p)
{
  m_binoppenalty = p;
}

bool MathTypesetter::insertPenalties() const
{
  return m_insert_penalties;
}

void MathTypesetter::setInsertPenalties(bool on)
{
  m_insert_penalties = on;
}

List MathTypesetter::mlist2hlist(MathList mlist, math::Style style)
{
  if (mlist.empty())
    return {};

  m_most_recent_atom = nullptr;
  m_current_style = style;

  preprocess(mlist);

  m_most_recent_atom = nullptr;

  List ret;

  for (auto it = mlist.begin(); it != mlist.end(); ++it)
  {
    auto current = *it;
    const bool isLast = current == mlist.back();
    std::shared_ptr<Node> next = (isLast ? nullptr : *std::next(it));
    const bool nextIsRel = next != nullptr && next->is<math::Atom>() && cast<math::Atom>(next)->type() == math::Atom::Rel;

    if (current->is<math::Atom>() && m_most_recent_atom != nullptr)
      insertSpace(ret, m_most_recent_atom, cast<math::Atom>(current));

    if (current->is<math::Atom>())
    {
      m_most_recent_atom = cast<math::Atom>(current);
      ret.push_back(m_most_recent_atom->nucleus());

      if (!isLast && insertPenalties() && (next == nullptr || !next->isPenalty()))
      {
        if (m_most_recent_atom->type() == math::Atom::Op && binoppenalty() != 0)
          ret.push_back(tex::penalty(binoppenalty()));
        else if (m_most_recent_atom->type() == math::Atom::Rel && relpenalty() != 0 && !nextIsRel)
          ret.push_back(tex::penalty(relpenalty()));
      }

    }
    else if (current->isBox() || current->is<Kern>() || current->isPenalty())
      ret.push_back(current);
  }

  return ret;
}

Font MathTypesetter::getFont(int fam) const
{
  return getFont(fam, m_current_style);
}

Font MathTypesetter::getFont(int fam, math::Style style) const
{
  if (style <= math::Style::SS)
  {
    return m_fonts[fam].scriptscriptfont;
  }
  else if (style <= math::Style::S)
  {
    return m_fonts[fam].scriptfont;
  }
  else
  {
    return m_fonts[fam].textfont;
  }
}

FontMetrics MathTypesetter::getMetrics(int fam) const
{
  return FontMetrics{ getFont(fam), engine().metrics() };
}

FontMetrics MathTypesetter::getMetrics(int fam, math::Style style) const
{
  return FontMetrics{ getFont(fam, style), engine().metrics() };
}

std::shared_ptr<Box> MathTypesetter::nullbox()
{
  static std::shared_ptr<Box> globalInstance = tex::hbox({});
  return globalInstance;
}

std::shared_ptr<Box> MathTypesetter::typeset(std::shared_ptr<MathSymbol> symbol)
{
  return engine().typeset(symbol, getFont(symbol->family()));
}

std::shared_ptr<Box> MathTypesetter::typesetDelimiter(const std::shared_ptr<Symbol>& ms, float minTotalHeight)
{
  if (ms == nullptr)
    return nullbox();

  return engine().typesetDelimiter(ms, minTotalHeight);
}

std::shared_ptr<VBox> MathTypesetter::radicalSignBox(float minTotalHeight)
{
  std::shared_ptr<Box> box = engine().typesetRadicalSign(minTotalHeight);
  auto ret = tex::vbox({ box });
  const float theta = getMetrics(XiFamily).defaultRuleThickness();
  if (ret->height() != theta)
  {
    VBoxEditor editor{ *ret };
    editor.changeHeight(theta);
    editor.done();
  }
  return ret;
}

std::shared_ptr<Box> MathTypesetter::boxit(std::shared_ptr<Node> node)
{
  if (node == nullptr)
  {
    return nullbox();
  }
  else if (node->isMathSymbol())
  {
    return typeset(cast<MathSymbol>(node));
  }
  else if (node->isBox())
  {
    return cast<Box>(node);
  }
  else if (node->is<MathListNode>())
  {
    MathList mlist = cast<MathListNode>(node)->list();
    MathTypesetter typesetter{ sharedEngine() };
    typesetter.setFonts(m_fonts);
    List hlist = typesetter.mlist2hlist(std::move(mlist), m_current_style);
    return tex::hbox(std::move(hlist));
  }

  throw std::runtime_error{ "boxit() : invalid input" };
}

std::shared_ptr<Box> MathTypesetter::boxit(std::shared_ptr<Node> node, math::Style s)
{
  RAIIStyleGuard style_guard{ m_current_style };
  m_current_style = s;
  return boxit(node);
}

std::shared_ptr<HBox> MathTypesetter::boxit(MathList mlist)
{
  return boxit(std::move(mlist), m_current_style);
}

std::shared_ptr<HBox> MathTypesetter::boxit(MathList mlist, math::Style s)
{
  MathTypesetter typesetter{ sharedEngine() };
  typesetter.setFonts(m_fonts);

  List hlist = typesetter.mlist2hlist(std::move(mlist), s);
  return tex::hbox(std::move(hlist));
}

std::shared_ptr<HBox> MathTypesetter::hboxit(std::shared_ptr<Node> node)
{
  auto box = boxit(node);
  if (!box->isHBox())
    return tex::hbox(List{ box });
  return cast<HBox>(box);
}


void MathTypesetter::preprocess(MathList& mlist)
{
  // Based on TeXbook, Appendix G

  RAIIStyleGuard style_guard{ m_current_style };

  for (auto it = mlist.begin(); it != mlist.end();)
  {
    auto& elem = *it;
    if (elem->isPenalty() || elem->is<Rule>() || elem->is<math::Boundary>())
    {
      // Rule 1
      ++it;
    }
    else if (elem->isGlue() || elem->isKern())
    {
      // Rule 2
      if (elem->isGlue())
      {
        rule2_translateglue(mlist, it);
      }
      else
      {
        ++it;
      }
    }
    else if (elem->is<math::StyleChange>())
    {
      // Rule 3
      m_current_style = elem->as<math::StyleChange>().style();
      it = mlist.erase(it);
    }
    // else if(elem->is<MathChoice>()) { } // Rule 4
    else if (elem->is<math::Atom>())
    {
      auto atom = cast<math::Atom>(elem);

      if (atom->type() == math::Atom::Bin)
      {
        rule5_binatom(mlist, it);
      }
      else if (atom->type() == math::Atom::Rel || atom->type() == math::Atom::Close || atom->type() == math::Atom::Punct)
      {
        // Rule 6
        if (m_most_recent_atom != nullptr && m_most_recent_atom->type() == math::Atom::Bin)
          m_most_recent_atom->changeType(math::Atom::Ord);
        rule17_processatom(mlist, it);
      }
      else if (atom->type() == math::Atom::Open || atom->type() == math::Atom::Inner)
      {
        // Rule 7
        rule17_processatom(mlist, it);
      }
      else if (atom->type() == math::Atom::Vcent)
      {
        rule8_vcent(mlist, it);
      }
      else if (atom->type() == math::Atom::Over)
      {
        rule9_over(mlist, it);
      }
      else if (atom->type() == math::Atom::Under)
      {
        rule10_underline(mlist, it);
      }
      else if (atom->type() == math::Atom::Rad)
      {
        rule11_radatom(mlist, it);
      }
      else if (atom->type() == math::Atom::Acc)
      {
        rule12_accatom(mlist, it);
      }
      else if (atom->type() == math::Atom::Op)
      {
        rule13_opatom(mlist, it);
      }
      else if (atom->type() == math::Atom::Ord)
      {
        // Rule 14 (incomplete)
        rule17_processatom(mlist, it);
      }
      else
      {
        assert(false);
      }

      m_most_recent_atom = atom;
      ++it;
    }
    else if (elem->is<math::Fraction>())
    {
      // Rule 15
      rule15_fraction(mlist, it);
      ++it;
    }
    else if (elem->is<math::Root>())
    {
      processRoot(mlist, it);
      m_most_recent_atom = cast<math::Atom>(*it);
      ++it;
    }
  }

  processBoundary(mlist);
}

void MathTypesetter::rule2_translateglue(MathList& mlist, MathList::iterator& current)
{
  auto g = std::static_pointer_cast<Glue>(*current);

  if (g->origin() == GlueOrigin::nonscript)
  {
    if (std::next(current) != mlist.end() && (*std::next(current))->isGlueOrKern())
    {
      if (m_current_style <= math::Style::S)
      {
        mlist.erase(std::next(current));
      }
    }
  }
  else if (g->origin() == GlueOrigin::mskip)
  {
    float factor = sigma<6>() / 18.f;
    g->spec().space *= factor;

    if (g->spec().shrinkOrder == GlueOrder::Normal)
      g->spec().shrink *= factor;

    if (g->spec().stretchOrder == GlueOrder::Normal)
      g->spec().stretch *= factor;
  }
 
  ++current;
}

void MathTypesetter::rule5_binatom(MathList& mlist, MathList::iterator& current)
{
  auto atom = std::static_pointer_cast<math::Atom>(*current);

  auto filter = [](math::Atom::Type t) -> bool {
    switch (t)
    {
    case math::Atom::Bin:
    case math::Atom::Op:
    case math::Atom::Rel:
    case math::Atom::Open: 
    case math::Atom::Punct:
      return true;
    default:
      return false;
    }
  };

  if (current == mlist.begin() && (m_most_recent_atom != nullptr && filter(m_most_recent_atom->type())))
  {
    atom->changeType(math::Atom::Ord);
    rule14(mlist, current);
  }
  else
  {
    rule17_processatom(mlist, current);
  }
}

void MathTypesetter::rule8_vcent(MathList& mlist, MathList::iterator& current)
{
  auto atom = std::static_pointer_cast<math::Atom>(*current);

  auto x = boxit(atom->nucleus());
  if (!x->is<VBox>())
    x = tex::vbox({ x });
  float v = x->totalHeight();
  float a = sigma<22>();
  {
    ListBoxEditor editor{ *cast<VBox>(x) };
    editor.setHeight(0.5f * v + a);
    editor.setDepth(0.5f * v - a);
  }
  atom->changeNucleus(x);
  atom->changeType(math::Atom::Ord);

  rule17_processatom(mlist, current);
}

void MathTypesetter::rule9_over(MathList& mlist, MathList::iterator& current)
{
  auto atom = std::static_pointer_cast<math::Atom>(*current);

  auto x = boxit(atom->nucleus(), m_current_style.cramp());
  float theta = xi<8>(); // default_rule_thickness
  auto box = tex::vbox({ kern(theta), hrule(x->width(), theta), kern(3 * theta), x });
  atom->changeNucleus(box);
  rule16_changeToOrd(mlist, current);
}

void MathTypesetter::rule10_underline(MathList& mlist, MathList::iterator& current)
{
  auto atom = std::static_pointer_cast<math::Atom>(*current);

  auto x = boxit(atom->nucleus(), m_current_style.cramp());
  float theta = xi<8>(); // default_rule_thickness
  auto box = tex::vtop({ x, kern(3 * theta), hrule(x->width(), theta) });
  {
    ListBoxEditor editor{ *box };
    editor.increaseDepth(theta);
  }
  atom->changeNucleus(box);
  rule16_changeToOrd(mlist, current);
}

void MathTypesetter::rule13_opatom(MathList& mlist, MathList::iterator& current)
{
  // @TODO: split with rule 13a

  auto atom = cast<math::Atom>(*current);
  assert(atom->type() == math::Atom::Op);

  const bool hasLimits = atom->limits() == math::Atom::Limits || (atom->limits() == math::Atom::DisplayLimits && m_current_style > math::Style::T);

  float delta = 0.f;

  if (atom->nucleus()->isMathSymbol() && m_current_style > math::Style::T)
  {
    auto mathsymbol = cast<MathSymbol>(atom->nucleus());

    auto x = engine().typesetLargeOp(mathsymbol);
    delta = getMetrics(mathsymbol->family()).italicCorrection(mathsymbol);
    const float a = getMetrics(SigmaFamily).axisHeight();
    auto boxed_x = tex::hbox({ x });
    boxed_x->setShiftAmount(0.5f * (x->height() - x->depth()) - a);
    atom->changeNucleus(tex::hbox({ x }));
  }

  if (!hasLimits)
  {
    rule17_processatom(mlist, current);
    return;
  }

  auto x = boxit(atom->superscript(), m_current_style.sup());
  auto y = boxit(atom->nucleus());
  auto z = boxit(atom->subscript(), m_current_style.sub());

  const float w = std::max({ x->width(), y->width(), z->width() });
  const std::shared_ptr<Glue> reboxGlue = tex::glue(0.f, tex::Stretch{ 1.0f, GlueOrder::Fil });
  if (x->width() < w)
    x = tex::hbox({ reboxGlue, x, reboxGlue }, w);
  if (y->width() < w)
    y = tex::hbox({ reboxGlue, y, reboxGlue }, w);
  if (z->width() < w)
    z = tex::hbox({ reboxGlue, z, reboxGlue }, w);

  List vlist;
  if (atom->superscript() != nullptr)
  {
    vlist.push_back(tex::kern(getMetrics(XiFamily).bigOpSpacing5()));
    cast<ListBox>(x)->shift(0.5f * delta);
    vlist.push_back(x);
    vlist.push_back(kern(std::max({ getMetrics(XiFamily).bigOpSpacing1(), getMetrics(XiFamily).bigOpSpacing3() - x->depth() })));
  }
  vlist.push_back(y);

  auto vbox = tex::vbox(std::move(vlist));
  const float h = vbox->totalHeight() - y->depth();

  if (atom->subscript() != nullptr)
  {
    VBoxEditor editor{ *vbox };
    editor.list().push_back(kern(std::max({ getMetrics(XiFamily).bigOpSpacing2(), getMetrics(XiFamily).bigOpSpacing4() - z->height() })));
    cast<ListBox>(z)->shift(-0.5f * delta);
    editor.list().push_back(z);
    editor.list().push_back(kern(getMetrics(XiFamily).bigOpSpacing5()));
    editor.rebox();
    editor.changeHeight(h);
    editor.done();
  }

  atom->changeNucleus(vbox);
}

void MathTypesetter::rule11_radatom(MathList& mathlist, MathList::iterator& current)
{
  std::shared_ptr<math::Atom> atom = cast<math::Atom>(*current);
  assert(atom->type() == math::Atom::Rad);

  auto x = boxit(atom->nucleus(), m_current_style.cramp());
  float theta = xi<8>(); // default_rule_thickness
  const float phi = m_current_style > math::Style::T ? sigma<5>() : theta;
  float psi = theta + 0.25f * std::abs(phi);
  auto y = radicalSignBox(psi + theta + x->totalHeight());
  theta = y->height();
  if (y->depth() > psi + y->totalHeight())
    psi = 0.5f * (psi + y->depth() - x->totalHeight());
  auto vbox = tex::vbox({ kern(theta), tex::hrule(x->width(), theta), kern(psi), x });
  y->setShiftAmount(y->shiftAmount() - psi + x->height()); // @TODO: looks suspicious
  atom->changeNucleus(tex::hbox({ y, vbox }));
  rule16_changeToOrd(mathlist, current);
}

void MathTypesetter::rule12_accatom(MathList& mathlist, MathList::iterator& current)
{
  std::shared_ptr<math::Atom> atom = cast<math::Atom>(*current);

  std::shared_ptr<Box> x = boxit(atom->nucleus(), m_current_style.cramp());
  const float u = x->width();
  float delta = std::min(x->height(), getMetrics(SigmaFamily).xHeight()); // @TODO: should be x-height in accent font

  if (atom->nucleus() != nullptr && atom->nucleus()->isDerivedFrom<Symbol>())
  {
    rule17_processatom(mathlist, current);
    atom->clearSubSupscripts();
    delta += (x->height() - cast<Box>(atom->nucleus())->height());
    x = cast<Box>(atom->nucleus());
  }

  /// TODO : add support for extensible accent !
  auto y = tex::hbox({ typeset(std::dynamic_pointer_cast<MathSymbol>(atom->accent())) });
  y->shift(0.5f * (u - y->width()));
  auto z = tex::vbox({ y, kern(-delta), x });
  if (z->height() < x->height())
  {
    VBoxEditor editor{ *z };
    editor.list().push_front(kern(x->height() - z->height()));
    editor.rebox();
    editor.done();
  }

  if (z->width() != x->width())
  {
    /// TODO: set z.width to x.width
  }

  atom->changeNucleus(z);
  rule16_changeToOrd(mathlist, current);
}

void MathTypesetter::rule16_changeToOrd(MathList& mathlist, MathList::iterator& current)
{
  auto atom = cast<math::Atom>(*current);
  atom->changeType(math::Atom::Ord);
  rule17_processatom(mathlist, current);
}

void MathTypesetter::rule14(MathList& mlist, MathList::iterator& current)
{
  // @TODO: implement this rule
  rule17_processatom(mlist, current);
}

void MathTypesetter::rule17_processatom(MathList& mathlist, MathList::iterator& current)
{
  // Rule 17

  auto atom = cast<math::Atom>(*current);

  atom->changeNucleus(boxit(atom->nucleus()));

  attachSubSup(mathlist, current);
}

bool MathTypesetter::isCharacterBox(const std::shared_ptr<Node>& node, float* w, float* h, float* d)
{
  std::shared_ptr<Box> cbox = std::dynamic_pointer_cast<tex::Box>(node);

  if (cbox == nullptr || cbox->isVBox())
    return false;

  if (node->isHBox())
  {
    auto hbox = cast<HBox>(node);
    const auto& hlist = hbox->list();
    if (hlist.size() != 2)
      return false;
    auto begin = hlist.begin();
    if ((*begin)->isBox() && !(*begin)->isHBox() && !(*begin)->isVBox())
    {
      begin++;
      if ((*begin)->isKern())
        cbox = cast<Box>(*hlist.begin());
    }
  }

  if (w != nullptr)
    *w = cbox->width();
  if (h != nullptr)
    *h = cbox->height();
  if (d != nullptr)
    *d = cbox->depth();
  return true;

  return true;
}


void MathTypesetter::attachSubSup(MathList& mlist, MathList::iterator& current)
{
  auto atom = cast<math::Atom>(*current);

  if (atom->subscript() == nullptr && atom->superscript() == nullptr)
    return;

  float u, v;
  if (isCharacterBox(atom->nucleus()))
    u = v = 0.f;
  else
  {
    const float q = sigma<18>(m_current_style.sup()); // sup_drop
    const float r = getMetrics(SigmaFamily, m_current_style.sub()).subDrop();
    auto box = cast<Box>(atom->nucleus());
    u = box->height() - q;
    v = box->depth() + r;
  }

  const float scriptspace = 0.5f * getMetrics(XiFamily).defaultRuleThickness();

  if (atom->superscript() == nullptr)
  {
    auto x = boxit(atom->subscript(), m_current_style.sub());
    if (isCharacterBox(x))
    {
      x = tex::hbox({ x, kern(scriptspace) });
    }
    else
    {
      ListBoxEditor editor{ *cast<ListBox>(x) };
      editor.enlarge(scriptspace);
    }

    float shift = std::max({ v, getMetrics(SigmaFamily).sub1(), x->height() - (4.f / 5.f) * std::abs(getMetrics(SigmaFamily).xHeight()) });
    cast<HBox>(x)->shift(shift);
    atom->changeNucleus(tex::hbox({ atom->nucleus(), x }));
    return;
  }

  auto x = boxit(atom->superscript(), m_current_style.sup());
  if (isCharacterBox(x))
  {
    x = tex::hbox({ x, kern(scriptspace) });
  }
  else
  {
    ListBoxEditor editor{ *cast<ListBox>(x) };
    editor.enlarge(scriptspace);
  }

  const float p = m_current_style == math::Style::D ? getMetrics(SigmaFamily).sup1() : (m_current_style == m_current_style.cramp() ? getMetrics(SigmaFamily).sup3() : getMetrics(SigmaFamily).sup2());
  u = std::max({ u, p, x->depth() + (4.f / 5.f) * std::abs(getMetrics(SigmaFamily).xHeight()) });

  if (atom->subscript() == nullptr)
  {
    cast<HBox>(x)->shift(-u);
    atom->changeNucleus(tex::hbox({ atom->nucleus(), x }));
    return;
  }

  auto y = boxit(atom->superscript(), m_current_style.sub());
  if (isCharacterBox(y))
  {
    y = tex::hbox({ y, kern(scriptspace) });
  }
  else
  {
    ListBoxEditor editor{ *cast<ListBox>(y) };
    editor.enlarge(scriptspace);
  }

  v = std::max({v, getMetrics(SigmaFamily).sub2()});

  const float theta = getMetrics(XiFamily).defaultRuleThickness();

  if ((u - x->depth()) - (y->height() - v) < 4 * theta)
  {
    v = 4.f * theta - (u - x->depth()) + y->height();
    const float psi = (4.f / 5.f) * getMetrics(SigmaFamily).xHeight() - (u - x->depth());
    if (psi > 0.f)
    {
      u += psi;
      v -= psi;
    }
  }


  float delta = 0.f;

  cast<ListBox>(x)->shift(delta);
  const float totalHeight = x->totalHeight() + u + v + y->totalHeight();
  auto vbox = tex::vbox({ x, kern(totalHeight - (x->totalHeight() + y->totalHeight())), y });
  {
    ListBoxEditor editor{ *vbox };
    editor.setHeight(x->height() + u);
    editor.setDepth(y->depth() + v);
  }
  atom->changeNucleus(tex::hbox({ atom->nucleus(), vbox }));
}


void MathTypesetter::rule15_fraction(MathList& mlist, MathList::iterator& current)
{
  std::shared_ptr<math::Fraction> frac = cast<math::Fraction>(*current);

  float theta = getMetrics(XiFamily).defaultRuleThickness();

  auto x = boxit(frac->numer(), m_current_style.fracNum());
  auto z = boxit(frac->denom(), m_current_style.fracDen());
  if (x->width() < z->width())
  {
    const std::shared_ptr<Glue> reboxGlue = tex::glue(0.f, tex::Stretch{ 1.0f, GlueOrder::Fil });
    x = tex::hbox({ reboxGlue, x, reboxGlue }, z->width());
  }
  else if (z->width() < x->width())
  {
    const std::shared_ptr<Glue> reboxGlue = tex::glue(0.f, tex::Stretch{ 1.0f, GlueOrder::Fil });
    z = tex::hbox({ reboxGlue, z, reboxGlue }, x->width());
  }
  const float w = z->width();

  float u;
  float v;
  if (m_current_style > math::Style::T)
  {
    u = getMetrics(SigmaFamily).num1();
    v = getMetrics(SigmaFamily).denom1();
  }
  else
  {
    u = getMetrics(SigmaFamily).num2();
    v = getMetrics(SigmaFamily).denom2();
  }

  // Rule 15d
  float phi = m_current_style > math::Style::T ? 3 * theta : theta;
  float a = getMetrics(SigmaFamily).axisHeight();
  if ((u - x->depth()) - (a + 0.5f * theta) < phi)
  {
    u += phi - ((u - x->depth()) - (a + 0.5f * theta));
  }
  if ((a - 0.5f * theta) - (z->height() - v) < phi)
  {
    v += phi - ((a - 0.5f * theta) - (z->height() - v));
  }

  const float h = x->height() + u;
  const float d = z->depth() + v;

  List vlist;
  vlist.push_back(x);
  vlist.push_back(kern(u - x->depth() - a - 0.5f * theta));
  vlist.push_back(hrule(w, theta));
  vlist.push_back(kern(a - 0.5f * theta + v - z->height()));
  vlist.push_back(z);

  auto vbox = tex::vbox(std::move(vlist));

  {
    tex::VBoxEditor editor{ *vbox };
    editor.changeHeight(h);
    editor.done();
  }

  assert(vbox->height() == h);
  //assert(vbox->depth() == d);

  *current = math::Atom::create<math::Atom::Inner>(vbox);
}

void MathTypesetter::processRoot(MathList& mlist, MathList::iterator& current)
{
  auto root = cast<math::Root>(*current);

  // Do as with a Rad atom
  auto x = boxit(root->radicand(), m_current_style.cramp());
  float theta = getMetrics(XiFamily).defaultRuleThickness();
  const float phi = m_current_style > math::Style::T ? getMetrics(SigmaFamily).xHeight() : 0.f;
  float psi = theta + 0.25f * std::abs(phi);
  auto y = radicalSignBox(psi + theta + x->totalHeight());
  const float radicalSignBoxWidth = y->width();
  theta = y->height();
  if (y->depth() > psi + y->totalHeight())
    psi = 0.5f * (psi + y->depth() - x->totalHeight());
  auto vbox = tex::vbox({ kern(theta), tex::hrule(x->width(), theta), kern(psi), x });
  y->setShiftAmount(y->shiftAmount() - psi + x->height());

  // Handle the root index
  auto index = boxit(root->degree(), math::Style::SS);

  float h = std::max(y->height() - y->shiftAmount(), vbox->height());
  float d = std::max(y->depth() + y->shiftAmount(), vbox->depth());

  // The amount the index is shifted by. This is taken from the TeX
  // source, in the definition of `\r@@t`.
  const float shiftAmount = 0.6f * (h - d);
  index->shift(-shiftAmount);

  auto atom = math::Atom::create<math::Atom::Rad>(tex::hbox({ index, kern(-radicalSignBoxWidth), y, vbox }));
  *current = atom;
  rule16_changeToOrd(mlist, current);
}

void MathTypesetter::processBoundary(MathList& mlist)
{
  static const float delimiterfactor = 901.f;
  static const float delimitershortfall = 0.5f;

  auto first = mlist.front();
  auto last = mlist.back();

  if (!first->is<math::Boundary>())
    return;

  assert(last->is<math::Boundary>());

  const float a = getMetrics(SigmaFamily).axisHeight();

  float h = 0.f;
  float d = 0.f;
  for (auto it = mlist.begin(); it != mlist.end(); ++it)
  {
    auto node = *it;
    if (node->is<math::Atom>())
      node = cast<math::Atom>(node)->nucleus();

    if (node->is<ListBox>())
    {
      h += cast<ListBox>(node)->height() - cast<ListBox>(node)->shiftAmount();
      d += cast<ListBox>(node)->depth() + cast<ListBox>(node)->shiftAmount();
    }
    else if (node->isBox())
    {
      h += cast<Box>(node)->height();
      d += cast<Box>(node)->depth();
    }
  }

  float delta = std::max(h - a, d + a);

  float tth = std::max(delimiterfactor * delta / 500.f, 2 * delta - delimitershortfall);
  auto left = tex::hbox({ typesetDelimiter(cast<math::Boundary>(first)->symbol(), tth) });
  auto right = tex::hbox({ typesetDelimiter(cast<math::Boundary>(last)->symbol(), tth) });
  left->setShiftAmount(0.5f * (left->height() - left->depth()) - a);
  right->setShiftAmount(0.5f * (right->height() - right->depth()) - a);

  (*mlist.begin()) = math::Atom::create<math::Atom::Open>(left);
  (*(--mlist.end())) = math::Atom::create<math::Atom::Close>(right);
}


void MathTypesetter::insertSpace(List& list, const std::shared_ptr<math::Atom>& preceding, const std::shared_ptr<math::Atom>& next)
{
  assert(static_cast<int>(preceding->type()) <= math::Atom::Inner);
  assert(static_cast<int>(next->type()) <= math::Atom::Inner);

  int amount = math::SpacingTable::spacing[preceding->type()][next->type()];
  bool nonScript = math::SpacingTable::spacingNonScript[preceding->type()][next->type()];
  if (amount == 0 || m_current_style <= math::Style::S && nonScript)
    return;
  else if (amount == 1)
    list.push_back(thinmuskip());
  else if (amount == 2)
    list.push_back(medmuskip());
  else if (amount == 3)
    list.push_back(thickmuskip());
}

std::shared_ptr<Glue> MathTypesetter::thinmuskip()
{
  const float mu = getMetrics(SigmaFamily).quad() / 18.f;
  return glue(3 * mu);
}

std::shared_ptr<Glue> MathTypesetter::medmuskip()
{
  const float mu = getMetrics(SigmaFamily).quad() / 18.f;
  return glue(4 * mu, tex::Shrink(4 * mu), tex::Stretch(2 * mu));
}

std::shared_ptr<Glue> MathTypesetter::thickmuskip()
{
  const float mu = getMetrics(SigmaFamily).quad() / 18.f;
  return tex::glue(5 * mu, tex::Stretch(5 * mu));
}

} // namespace tex

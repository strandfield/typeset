// Copyright (C) 2018 Vincent Chambrin
// This file is part of the liblayout project
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
#include "tex/math/fraction.h"
#include "tex/math/style.h"

#include <algorithm>
#include <cassert>

namespace tex
{

namespace math
{

std::shared_ptr<Box> nullbox()
{
  static std::shared_ptr<Box> globalInstance = tex::hbox({});
  return globalInstance;
}

std::shared_ptr<Box> typeset(std::shared_ptr<Symbol> symbol, const Options & opts)
{
  return opts.engine()->typeset(symbol, opts.font(), opts.fontSize());
}

static std::shared_ptr<VBox> radicalSignBox(float minTotalHeight, const Options & opts)
{
  std::shared_ptr<Box> box = opts.engine()->typesetRadicalSign(minTotalHeight, opts.fontSize());
  auto ret = tex::vbox({ box });
  const float theta = opts.fontMetrics().defaultRuleThickness();
  if (ret->height() != theta)
  {
    VBoxEditor editor{ *ret };
    editor.changeHeight(theta);
    editor.done();
  }
  return ret;
}

static std::shared_ptr<Box> boxit(std::shared_ptr<Node> node, const Options & opts)
{
  if (node == nullptr)
  {
    return nullbox();
  }
  else if (node->isDerivedFrom<Symbol>())
  {
    return typeset(cast<Symbol>(node), opts);
  }
  else if (node->isBox())
  {
    return cast<Box>(node);
  }
  else if (node->is<MathListNode>())
  {
    MathList mlist = cast<MathListNode>(node)->list();
    List hlist = mlist_to_hlist(std::move(mlist), opts);
    return tex::hbox(std::move(hlist));
  }

  throw std::runtime_error{ "boxit() : invalid input" };
}

static std::shared_ptr<Box> boxit(MathList mlist, const Options & opts)
{
  List hlist = mlist_to_hlist(std::move(mlist), opts);
  return tex::hbox(std::move(hlist));
}

static std::shared_ptr<HBox> hboxit(std::shared_ptr<Node> node, const Options & opts)
{
  auto box = boxit(node, opts);
  if (!box->is<HBox>())
    return tex::hbox(List{ box });
  return cast<HBox>(box);
}


static void changeToOrd(MathList & mathlist, MathList::iterator & current, const Options & opts);
static void attachSubSup(MathList & mlist, MathList::iterator & current, const Options & opts);
static void processAtom(MathList & mathlist, MathList::iterator & current, const Options & opts);

static void processOpAtom(MathList & mathlist, MathList::iterator & current, const Options & opts)
{
  auto atom = cast<Atom>(*current);
  assert(atom->type() == Atom::Op);

  const bool hasLimits = atom->limits() == Atom::Limits || (atom->limits() == Atom::DisplayLimits && opts.mathStyle() > Style::T);

  float delta = 0.f;
  if (atom->nucleus()->isDerivedFrom<Symbol>() && opts.mathStyle() > Style::T)
  {
    auto mathsymbol = cast<Symbol>(atom->nucleus());

    auto x = opts.engine()->typesetLargeOp(mathsymbol, opts.fontSize());
    delta = opts.fontMetrics().italicCorrection(mathsymbol);
    const float a = opts.fontMetrics().axisHeight();
    auto boxed_x = tex::hbox({ x });
    boxed_x->setShiftAmount(0.5f * (x->height() - x->depth()) - a);
    atom->changeNucleus(tex::hbox({ x }));
  }

  if (!hasLimits)
  {
    processAtom(mathlist, current, opts);
    return;
  }

  auto x = boxit(atom->superscript(), opts.withStyle(opts.mathStyle().sup()));
  auto y = boxit(atom->nucleus(), opts);
  auto z = boxit(atom->subscript(), opts.withStyle(opts.mathStyle().sub()));

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
    vlist.push_back(tex::kern(opts.fontMetrics().bigOpSpacing5()));
    cast<ListBox>(x)->shift(0.5f * delta);
    vlist.push_back(x);
    vlist.push_back(kern(std::max(opts.fontMetrics().bigOpSpacing1(), opts.fontMetrics().bigOpSpacing3() - x->depth())));
  }
  vlist.push_back(y);

  auto vbox = tex::vbox(std::move(vlist));
  const float h = vbox->totalHeight() - y->depth();

  if (atom->subscript() != nullptr)
  {
    VBoxEditor editor{ *vbox };
    editor.list().push_back(kern(std::max(opts.fontMetrics().bigOpSpacing2(), opts.fontMetrics().bigOpSpacing4() - z->height())));
    cast<ListBox>(z)->shift(-0.5f * delta);
    editor.list().push_back(z);
    editor.list().push_back(kern(opts.fontMetrics().bigOpSpacing5()));
    editor.rebox();
    editor.changeHeight(h);
    editor.done();
  }

  atom->changeNucleus(vbox);
}


static void processRadAtom(MathList & mathlist, MathList::iterator & current, const Options & opts)
{
  std::shared_ptr<Atom> atom = cast<Atom>(*current);
  assert(atom->type() == Atom::Rad);

  auto x = boxit(atom->nucleus(), opts.withCrampedStyle());
  float theta = opts.fontMetrics().defaultRuleThickness();
  const float phi = opts.mathStyle() > Style::T ? opts.fontMetrics().xHeight() : 0.f;
  float psi = theta + 0.25f * std::abs(phi);
  auto y = radicalSignBox(psi + theta + x->totalHeight(), opts);
  const float radicalSignBoxWidth = y->width();
  theta = y->height();
  if (y->depth() > psi + y->totalHeight())
    psi = 0.5f * (psi + y->depth() - x->totalHeight());
  auto vbox = tex::vbox({ kern(theta), tex::hrule(x->width(), theta), kern(psi), x });
  y->setShiftAmount(y->shiftAmount() - psi + x->height());


  if (atom->index() == nullptr)
  {
    atom->changeNucleus(tex::hbox({ y, vbox }));
    changeToOrd(mathlist, current, opts);
    return;
  }

  // Handle the optional root index
  auto index = hboxit(atom->index(), opts.withStyle(Style::SS));

  float h = std::max(y->height() - y->shiftAmount(), vbox->height());
  float d = std::max(y->depth() + y->shiftAmount(), vbox->depth());

  // The amount the index is shifted by. This is taken from the TeX
  // source, in the definition of `\r@@t`.
  const float shiftAmount = 0.6f * (h - d);
  index->shift(-shiftAmount);

  atom->changeNucleus(tex::hbox({ index, kern(-radicalSignBoxWidth), y, vbox }));
  changeToOrd(mathlist, current, opts);
}

static void processAccAtom(MathList & mathlist, MathList::iterator & current, const Options & opts)
{
  std::shared_ptr<Atom> atom = cast<Atom>(*current);

  std::shared_ptr<Box> x = boxit(atom->nucleus(), opts.withCrampedStyle());
  const float u = x->width();
  float delta = std::min(x->height(), opts.fontMetrics().xHeight());

  if (atom->nucleus() != nullptr && atom->nucleus()->isDerivedFrom<Symbol>())
  {
    processAtom(mathlist, current, opts);
    atom->clearSubSupscripts();
    delta += (x->height() - cast<Box>(atom->nucleus())->height());
    x = cast<Box>(atom->nucleus());
  }

  /// TODO : add support for extensible accent !
  auto y = tex::hbox({ typeset(atom->accent(), opts) });
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
  changeToOrd(mathlist, current, opts);
}

static void changeToOrd(MathList & mathlist, MathList::iterator & current, const Options & opts)
{
  auto atom = cast<Atom>(*current);
  atom->changeType(Atom::Ord);
  processAtom(mathlist, current, opts);
}

static void processAtom(MathList & mathlist, MathList::iterator & current, const Options & opts)
{
  auto atom = cast<Atom>(*current);

  atom->changeNucleus(boxit(atom->nucleus(), opts));

  attachSubSup(mathlist, current, opts);
}

static bool isCharacterBox(const std::shared_ptr<Node> & node, float *w = nullptr, float *h = nullptr, float *d = nullptr)
{
  std::shared_ptr<Box> cbox = std::dynamic_pointer_cast<tex::Box>(node);

  if (cbox == nullptr || cbox->is<VBox>())
    return false;

  if (node->is<HBox>())
  {
    auto hbox = cast<HBox>(node);
    const auto & hlist = hbox->list();
    if (hlist.size() != 2)
      return false;
    auto begin = hlist.begin();
    if ((*begin)->isDerivedFrom<Box>() && !(*begin)->is<HBox>() && !(*begin)->is<VBox>())
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


static void attachSubSup(MathList & mlist, MathList::iterator & current, const Options & opts)
{
  auto atom = cast<Atom>(*current);

  if (atom->subscript() == nullptr && atom->superscript() == nullptr)
    return;

  float u, v;
  if (isCharacterBox(atom->nucleus()))
    u = v = 0.f;
  else
  {
    const float q = opts.withStyle(opts.mathStyle().sup()).fontMetrics().supDrop();
    const float r = opts.withStyle(opts.mathStyle().sub()).fontMetrics().subDrop();
    auto box = cast<Box>(atom->nucleus());
    u = box->height() - q;
    v = box->depth() + r;
  }


  const float scriptspace = 0.5f * opts.fontMetrics().defaultRuleThickness();

  if (atom->superscript() == nullptr)
  {
    auto x = boxit(atom->subscript(), opts.withStyle(opts.mathStyle().sub()));
    if (isCharacterBox(x))
    {
      x = tex::hbox({ x, kern(scriptspace) });
    }
    else
    {
      ListBoxEditor editor{ *cast<ListBox>(x) };
      editor.enlarge(scriptspace);
    }

    float shift = std::max({ v, opts.fontMetrics().sub1(), x->height() - (4.f / 5.f) * std::abs(opts.fontMetrics().xHeight()) });
    cast<HBox>(x)->shift(shift);
    atom->changeNucleus(tex::hbox({ atom->nucleus(), x }));
    return;
  }

  auto x = boxit(atom->superscript(), opts.withStyle(opts.mathStyle().sup()));
  if (isCharacterBox(x))
  {
    x = tex::hbox({ x, kern(scriptspace) });
  }
  else
  {
    ListBoxEditor editor{ *cast<ListBox>(x) };
    editor.enlarge(scriptspace);
  }

  const float p = opts.mathStyle() == Style::D ? opts.fontMetrics().sup1() : (opts.mathStyle() == opts.mathStyle().cramp() ? opts.fontMetrics().sup3() : opts.fontMetrics().sup2());
  u = std::max({ u, p, x->depth() + (4.f / 5.f) * std::abs(opts.fontMetrics().xHeight()) });

  if (atom->subscript() == nullptr)
  {
    cast<HBox>(x)->shift(-u);
    atom->changeNucleus(tex::hbox({ atom->nucleus(), x }));
    return;
  }

  auto y = boxit(atom->superscript(), opts.withStyle(opts.mathStyle().sub()));
  if (isCharacterBox(y))
  {
    y = tex::hbox({ y, kern(scriptspace) });
  }
  else
  {
    ListBoxEditor editor{ *cast<ListBox>(y) };
    editor.enlarge(scriptspace);
  }

  v = std::max(v, opts.fontMetrics().sub2());

  const float theta = opts.fontMetrics().defaultRuleThickness();

  if ((u - x->depth()) - (y->height() - v) < 4 * theta)
  {
    v = 4.f * theta - (u - x->depth()) + y->height();
    const float psi = (4.f / 5.f) * opts.fontMetrics().xHeight() - (u - x->depth());
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


static void processFraction(MathList & mlist, MathList::iterator & current, const Options & opts)
{
  std::shared_ptr<Fraction> frac = cast<Fraction>(*current);

  float theta = opts.fontMetrics().defaultRuleThickness();


  auto x = boxit(frac->numer(), opts.withStyle(opts.mathStyle().fracNum()));
  auto z = boxit(frac->denom(), opts.withStyle(opts.mathStyle().fracDen()));
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
  if (opts.mathStyle() > Style::T)
  {
    u = opts.fontMetrics().num1();
    v = opts.fontMetrics().denom1();
  }
  else
  {
    u = opts.fontMetrics().num2();
    v = opts.fontMetrics().denom2();
  }

  // Rule 15d
  float phi = opts.mathStyle() > Style::T ? 3 * theta : theta;
  float a = opts.fontMetrics().axisHeight();
  if ((u - x->depth()) - (a + 0.5f * theta) < phi)
  {
    u += phi - (u - x->depth()) - (a + 0.5f * theta);
  }
  if ((a - 0.5f * theta) - (z->height() - v) < phi)
  {
    v += phi - ((a - 0.5f * theta) - (z->height() - v));
  }

  float h = x->height() + u;
  float d = z->depth() + v;

  List vlist;
  vlist.push_back(x);
  vlist.push_back(kern((a - 0.5f * theta) - (h - x->totalHeight())));
  vlist.push_back(hrule(w, theta));
  vlist.push_back(kern(d - z->totalHeight() - (a - 0.5f * theta)));
  vlist.push_back(z);

  *current = Atom::create<Atom::Inner>(tex::vbox(std::move(vlist)));
}

static std::shared_ptr<Box> typesetDelimiter(const std::shared_ptr<Symbol> & ms, float minTotalHeight, const Options & opts)
{
  if (ms == nullptr)
    return nullbox();

  return opts.engine()->typesetDelimiter(ms, minTotalHeight, opts.fontSize());
}

void processBoundary(MathList & mlist, const Options & opts)
{
  static const float delimiterfactor = 901.f;
  static const float delimitershortfall = 0.5f;

  auto first = mlist.front();
  auto last = mlist.back();

  if (!first->is<Boundary>())
    return;

  assert(last->is<Boundary>());

  const float a = opts.fontMetrics().axisHeight();

  float h = 0.f;
  float d = 0.f;
  for (auto it = mlist.begin(); it != mlist.end(); ++it)
  {
    auto node = *it;
    if (node->is<Atom>())
      node = cast<Atom>(node)->nucleus();

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
  auto left = tex::hbox({ typesetDelimiter(cast<Boundary>(first)->symbol(), tth, opts) });
  auto right = tex::hbox({ typesetDelimiter(cast<Boundary>(last)->symbol(), tth, opts) });
  left->setShiftAmount(0.5f * (left->height() - left->depth()) - a);
  right->setShiftAmount(0.5f * (right->height() - right->depth()) - a);

  (*mlist.begin()) = Atom::create<Atom::Open>(left);
  (*(--mlist.end())) = Atom::create<Atom::Close>(right);
}


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

static std::shared_ptr<Glue> thinmuskip(const Options & opts)
{
  const float mu = opts.fontMetrics().quad() / 18.f;
  return glue(3 * mu);
}

static std::shared_ptr<Glue> medmuskip(const Options & opts)
{
  const float mu = opts.fontMetrics().quad() / 18.f;
  return glue(4 * mu, tex::Shrink(4 * mu), tex::Stretch(2 * mu));
}

static std::shared_ptr<Glue> thickmuskip(const Options & opts)
{
  const float mu = opts.fontMetrics().quad() / 18.f;
  return tex::glue(5 * mu, tex::Stretch(5 * mu));
}

void insertSpace(List & list, const std::shared_ptr<Atom> & preceding, const std::shared_ptr<Atom> & next, const Options & opts)
{
  assert(static_cast<int>(preceding->type()) <= Atom::Inner);
  assert(static_cast<int>(next->type()) < Atom::Inner);

  int amount = SpacingTable::spacing[preceding->type()][next->type()];
  bool nonScript = SpacingTable::spacingNonScript[preceding->type()][next->type()];
  if (amount == 0 || opts.mathStyle() <= Style::S && nonScript)
    return;
  else if (amount == 1)
    list.push_back(thinmuskip(opts));
  else if (amount == 2)
    list.push_back(medmuskip(opts));
  else if (amount == 3)
    list.push_back(thickmuskip(opts));
}

inline static bool any_of(Atom::Type t, std::initializer_list<Atom::Type> types)
{
  return std::find(types.begin(), types.end(), t) != types.end();
}

void preprocess(MathList &mathlist, const Options & opts)
{
  auto it = mathlist.begin();

  bool firstAtom = true;
  std::shared_ptr<Atom> mostRecentAtom = nullptr;

  for (; it != mathlist.end(); ++it)
  {
    auto & elem = *it;
    if (elem->isPenalty() || elem->is<Rule>() || elem->is<Boundary>())
      continue;
    else if (elem->isGlue() || elem->isKern())
      continue;
    else if (elem->is<Atom>())
    {
      auto atom = cast<Atom>(elem);
      if (atom->type() == Atom::Bin || firstAtom || (mostRecentAtom != nullptr && any_of(mostRecentAtom->type(), { Atom::Bin, Atom::Op, Atom::Rel, Atom::Open, Atom::Punct })))
      {
        firstAtom = false;
        atom->changeType(Atom::Ord);
        processAtom(mathlist, it, opts);
      }
      else if (any_of(atom->type(), { Atom::Rel, Atom::Close, Atom::Punct }))
      {
        if (mostRecentAtom != nullptr && mostRecentAtom->type() == Atom::Bin)
          mostRecentAtom->changeType(Atom::Ord);
        processAtom(mathlist, it, opts);
      }
      else if (any_of(atom->type(), { Atom::Open, Atom::Inner }))
      {
        processAtom(mathlist, it, opts);
      }
      else if (atom->type() == Atom::Vcent)
      {
        auto x = boxit(atom->nucleus(), opts);
        if (!x->is<VBox>())
          x = tex::vbox({ x });
        float v = x->totalHeight();
        float a = opts.fontMetrics().axisHeight();
        {
          ListBoxEditor editor{ *cast<VBox>(x) };
          editor.setHeight(0.5f * v + a);
          editor.setDepth(0.5f * v - a);
        }
        atom->changeNucleus(x);
        atom->changeType(Atom::Ord);
        processAtom(mathlist, it, opts);
      }
      else if (atom->type() == Atom::Over)
      {
        auto x = boxit(atom->nucleus(), opts.withCrampedStyle());
        float theta = opts.fontMetrics().defaultRuleThickness();
        auto box = tex::vbox({ kern(theta), hrule(x->width(), theta), kern(3 * theta), x });
        atom->changeNucleus(box);
        changeToOrd(mathlist, it, opts);
      }
      else if (atom->type() == Atom::Under)
      {
        auto x = boxit(atom->nucleus(), opts.withCrampedStyle());
        float theta = opts.fontMetrics().defaultRuleThickness();
        auto box = tex::vtop({ x, kern(3 * theta), hrule(x->width(), theta) });
        {
          ListBoxEditor editor{ *box };
          editor.increaseDepth(theta);
        }
        atom->changeNucleus(box);
        changeToOrd(mathlist, it, opts);
      }
      else if (atom->type() == Atom::Rad)
      {
        processRadAtom(mathlist, it, opts);
      }
      else if (atom->type() == Atom::Acc)
      {
        processAccAtom(mathlist, it, opts);
      }
      else if (atom->type() == Atom::Op)
      {
        processOpAtom(mathlist, it, opts);
      }
      else
      {
        assert(false);
      }
    }
    else if (elem->is<Fraction>())
    {
      processFraction(mathlist, it, opts);
    }
  }

  processBoundary(mathlist, opts);
}

List mlist_to_hlist(MathList && mathlist, const Options & opts, int relpenalty, int binoppenalty)
{
  preprocess(mathlist, opts);

  List ret;
  std::shared_ptr<Atom> mostRecentAtom = nullptr;
  for (auto it = mathlist.begin(); it != mathlist.end(); ++it)
  {
    auto current = *it;
    const bool isLast = current == mathlist.back();
    std::shared_ptr<Node> next = (isLast ? nullptr : *std::next(it));
    const bool nextIsRel = next != nullptr && next->is<Atom>() && cast<Atom>(next)->type() == Atom::Rel;

    if (current->is<Atom>() && mostRecentAtom != nullptr)
      insertSpace(ret, mostRecentAtom, cast<Atom>(current), opts);

    if (current->is<Atom>())
    {
      mostRecentAtom = cast<Atom>(current);
      ret.push_back(mostRecentAtom->nucleus());

      if (!isLast && (next == nullptr || !next->isPenalty()))
      {
        if (mostRecentAtom->type() == Atom::Op && binoppenalty != 0)
          ret.push_back(tex::penalty(binoppenalty));
        else if (mostRecentAtom->type() == Atom::Rel && relpenalty != 0 && !nextIsRel)
          ret.push_back(tex::penalty(relpenalty));
      }

    }
    else if (current->isDerivedFrom<Box>() || current->is<Kern>() || current->isPenalty())
      ret.push_back(current);
  }

  return ret;
}

std::shared_ptr<HBox> typeset(MathList & mathlist, const Options & opts)
{
  List hlist = mlist_to_hlist(std::move(mathlist), opts);
  return tex::hbox(std::move(hlist));
}

} // namespace math

} // namespace tex

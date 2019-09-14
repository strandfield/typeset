// Copyright (C) 2019 Vincent Chambrin
// This file is part of the typeset project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "test.h"

#include "tex/math/atom.h"

void test_atom_creation()
{
  using namespace tex;

  auto x = std::make_shared<Symbol>();
  auto dot = std::make_shared<Symbol>();
  auto y = std::make_shared<Symbol>();
  auto z = std::make_shared<Symbol>();

  auto acc = math::Atom::create<math::Atom::Acc>(x, dot);
  ASSERT_EQ(acc->type(), math::Atom::Acc);
  ASSERT_EQ(acc->nucleus(), x);
  ASSERT_EQ(acc->accent(), dot);
  ASSERT_EQ(acc->limits(), math::Atom::NoLimits);

  auto rad = math::Atom::create<math::Atom::Rad>(x, y);
  ASSERT_EQ(rad->type(), math::Atom::Rad);
  ASSERT_EQ(rad->nucleus(), x);
  ASSERT_EQ(rad->index(), y);
  ASSERT_EQ(rad->limits(), math::Atom::NoLimits);

  auto ord = math::Atom::create<math::Atom::Ord>(x, y, z);
  ASSERT_EQ(ord->type(), math::Atom::Ord);
  ASSERT_EQ(ord->nucleus(), x);
  ASSERT_EQ(ord->subscript(), y);
  ASSERT_EQ(ord->superscript(), z);
}


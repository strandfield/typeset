// Copyright (C) 2018 Vincent Chambrin
// This file is part of the Yasl project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/math/atom.h"

#include <cassert>
#include <iostream>
#include <type_traits>

#define ASSERT_EQ(a, b) assert((a) == (b))
#define ASSERT_TRUE(a) assert((a))
#define ASSERT_FALSE(a) assert(!(a))

void test_atom_creation()
{
  using namespace tex;

  std::cout << "Test: Atom creation" << std::endl;

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

  std::cout << "...passed" << std::endl;
}


int main(int argc, char *argv[])
{
  test_atom_creation();

  return 0;
}

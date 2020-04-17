// Copyright (C) 2019-2020 Vincent Chambrin
// This file is part of the typeset project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "catch.hpp"

#include "tex/math/atom.h"

TEST_CASE("Helper functions for creating atoms are working", "[atom]")
{
  using namespace tex;

  auto x = std::make_shared<Symbol>();
  auto dot = std::make_shared<Symbol>();
  auto y = std::make_shared<Symbol>();
  auto z = std::make_shared<Symbol>();

  auto acc = math::Atom::create<math::Atom::Acc>(x, dot);
  REQUIRE(acc->type() == math::Atom::Acc);
  REQUIRE(acc->nucleus() == x);
  REQUIRE(acc->accent() == dot);
  REQUIRE(acc->limits() == math::Atom::NoLimits);

  auto rad = math::Atom::create<math::Atom::Rad>(x);
  REQUIRE(rad->type() == math::Atom::Rad);
  REQUIRE(rad->nucleus() == x);
  REQUIRE(rad->limits() == math::Atom::NoLimits);

  auto ord = math::Atom::create<math::Atom::Ord>(x, y, z);
  REQUIRE(ord->type() == math::Atom::Ord);
  REQUIRE(ord->nucleus() == x);
  REQUIRE(ord->subscript() == y);
  REQUIRE(ord->superscript() == z);
}


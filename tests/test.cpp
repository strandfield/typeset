// Copyright (C) 2019 Vincent Chambrin
// This file is part of the typeset project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "test.h"

#include "tex/math/atom.h"

#include <type_traits>

int test_exit_code = 0;

int run_test(void(*f)(void))
{
  test_exit_code = 0;

  f();

  return test_exit_code;
}

void test_atom_creation(); // test-atom.cpp
void test_lexer(); // test-lexer.cpp
void test_parser_1(); // test-parser.cpp
void test_parser_2(); // test-parser.cpp
void test_parser_3(); // test-parser.cpp
void test_machine_1(); // test-machine.cpp
void test_machine_2(); // test-machine.cpp
void test_machine_3(); // test-machine.cpp

int main(int argc, char *argv[])
{
  int ec = 0;

  ec += run_test(test_atom_creation);
  ec += run_test(test_lexer);
  ec += run_test(test_parser_1);
  ec += run_test(test_parser_2);
  ec += run_test(test_machine_1);
  ec += run_test(test_machine_2);
  ec += run_test(test_machine_3);

  return ec;
}

// Copyright (C) 2019 Vincent Chambrin
// This file is part of the typeset project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "test.h"

#include "tex/math/atom.h"

#include <type_traits>
#include <vector>

int test_exit_code = 0;

int run_test(void(*f)(void))
{
  test_exit_code = 0;

  f();

  return test_exit_code;
}

struct Test
{
  std::string name;
  void (*function)(void);
};

std::vector<Test>& testList()
{
  static std::vector<Test> static_instance = {};
  return static_instance;
}

int register_test(const char* name, void(*f)(void))
{
  testList().push_back({ std::string(name), f });
  return testList().size() - 1;
}

int run_tests()
{
  int n = 0;

  const std::vector<Test>& tests = testList();

  for (const auto& t : tests)
  {
    std::cout << "Running " << t.name << "\n";
    n += run_test(t.function);
  }

  return n;
}

int main(int argc, char *argv[])
{
  int ec = 0;

  ec += run_tests();

  return ec;
}

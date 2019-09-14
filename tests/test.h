// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_TEST_H
#define LIBTYPESET_TEST_H

#include <iostream>

extern int test_exit_code;

#define ASSERT(x) do { if(!(x)) { std::cerr << "FAIL! " << #x << " (file:" << __FILE__ << ":" << __LINE__ << ")" << std::endl; test_exit_code = 1; return; } } while(false)

#define ASSERT_EQ(a, b) ASSERT(((a) == (b)))
#define ASSERT_NEQ(a, b) ASSERT(((a) != (b)))

int run_test(void(*f)(void));

#endif // LIBTYPESET_TEST_H

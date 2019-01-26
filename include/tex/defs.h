// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_DEFS_H
#define LIBTYPESET_DEFS_H

#if (defined(WIN32) || defined(_WIN32)) && !defined(LIBTYPESET_STATIC_LINKING)

#if defined(LIBTYPESET_BUILD_LIB)
#  define LIBTYPESET_API __declspec(dllexport)
#else
#  define LIBTYPESET_API __declspec(dllimport)
#endif

#else

#  define LIBTYPESET_API

#endif

#endif // LIBTYPESET_DEFS_H

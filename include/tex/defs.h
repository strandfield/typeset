
#ifndef LIBLAYOUT_DEFS_H
#define LIBLAYOUT_DEFS_H

#if defined(WIN32) || defined(_WIN32)

#if defined(LIBLAYOUT_BUILD_LIB)
#  define LIBLAYOUT_API __declspec(dllexport)
#else
#  define LIBLAYOUT_API __declspec(dllimport)
#endif

#else

#  define LIBLAYOUT_API

#endif

#endif // LIBLAYOUT_DEFS_H

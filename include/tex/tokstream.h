// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_TOKSTREAM_H
#define LIBTYPESET_TOKSTREAM_H

#include "tex/token.h"

#include <vector>

namespace tex
{

namespace parsing
{

inline Token read(std::vector<Token>& toks)
{
  Token t = toks.front();
  toks.erase(toks.begin());
  return t;
}

inline void read(std::vector<Token>& toks, int n)
{
  toks.erase(toks.begin(), toks.begin() + n);
}

inline void discard(std::vector<Token>& toks, int n = 1)
{
  toks.erase(toks.begin(), toks.begin() + n);
}

inline const Token& peek(const std::vector<Token>& toks)
{
  return toks.front();
}

inline const Token& peek(const std::vector<Token>& toks, size_t n)
{
  return *(toks.begin() + n);
}

inline void write(Token&& tok, std::vector<Token>& out)
{
  out.push_back(std::move(tok));
}

} // namespace parsing

} // namespace tex

#endif // LIBTYPESET_TOKSTREAM_H

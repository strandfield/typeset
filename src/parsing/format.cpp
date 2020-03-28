// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/parsing/format.h"

#include "tex/lexer.h"

#include <cassert>

namespace tex
{

namespace parsing
{

enum Mnemonic
{
  End = 'e',
  Skip = '>',
  Def = 'd',
  Repl = 'r',
  Param = 'p',
  Tok = 't',
  Cs = 'c',
  Save = 's',
};

class FormatLoader
{
public:
  const char* src = nullptr;
  const char* bytecode = nullptr;
  int m_version = 0;
  std::vector<Macro> m_result;
  std::string m_cur_name;
  bool m_reading_param_text = false;
  std::vector<Token> m_cur_param_text;
  std::vector<Token> m_cur_repl_seq;

  void load();

private:
  void skip();
  void def();
  void repl();
  void param();
  void tok();
  void cs();
  void save();

  void dispatch(Token tok);
};

void FormatLoader::load()
{
  char v = *(bytecode++);
  assert(v == 'v');
  m_version = static_cast<int>(*(bytecode++) - '0');

  while (*bytecode != Mnemonic::End)
  {
    switch (*(bytecode++))
    {
    case Mnemonic::Skip:
      skip();
      break;
    case Mnemonic::Def:
      def();
      break;
    case Mnemonic::Repl:
      repl();
      break;
    case Mnemonic::Param:
      param();
      break;
    case Mnemonic::Tok:
      tok();
      break;
    case Mnemonic::Cs:
      cs();
      break;
    case Mnemonic::Save:
      save();
      break;
    }
  }
}

void FormatLoader::skip()
{
  src += static_cast<size_t>(*(bytecode++) - '0');
}

void FormatLoader::def()
{
  src += 4; // skip \def

  if (*src == '\\')
    ++src;

  size_t size = static_cast<size_t>(*(bytecode++) - '0');
  m_cur_name.clear();
  m_cur_name.assign(src, size);
  src += size;
  m_reading_param_text = true;
}

void FormatLoader::repl()
{
  m_reading_param_text = false;
  ++src;
}

void FormatLoader::param()
{
  if (*src == '#')
    ++src;

  dispatch(Token{ static_cast<int>(*(src++) - '0') });
}

void FormatLoader::tok()
{
  CharacterToken ctok;
  ctok.value = *(src++);
  ctok.category = Lexer::DefaultCatCodes[static_cast<size_t>(ctok.value)];

  dispatch(Token{ ctok });
}

void FormatLoader::cs()
{
  if (*src == '\\')
    ++src;

  size_t size = static_cast<size_t>(*(bytecode++) - '0');

  std::string cs{ src, src + size };
  src += size;

  dispatch(Token{ std::move(cs) });
}

void FormatLoader::save()
{
  Macro m{ std::move(m_cur_name), std::move(m_cur_param_text), std::move(m_cur_repl_seq) };
  m_result.push_back(std::move(m));
  ++src;
}

void FormatLoader::dispatch(Token tok)
{
  if (m_reading_param_text)
    m_cur_param_text.push_back(std::move(tok));
  else
    m_cur_repl_seq.push_back(std::move(tok));
}

Format::Format(const char* src, const char* bytecode)
  : m_src(src),
  m_bytecode(bytecode)
{

}

const char* Format::source() const
{
  return m_src;
}

const char* Format::bytecode() const
{
  return m_bytecode;
}

std::vector<Macro> Format::load(const Format& fmt)
{
  FormatLoader loader;
  loader.src = fmt.source();
  loader.bytecode = fmt.bytecode();

  loader.load();

  return loader.m_result;
}

} // namespace parsing

} // namespace tex

// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_PARSING_PREPROCESSOR_H
#define LIBTYPESET_PARSING_PREPROCESSOR_H

#include "tex/tokstream.h"

#include <list>
#include <map>
#include <vector>

namespace tex
{

namespace parsing
{

class InputStream;
struct Memory;

class LIBTYPESET_API Macro
{
public:
  Macro() = default;
  Macro(const Macro&) = default;
  Macro(Macro&&) = default;

  Macro(std::string cs, std::vector<Token>&& repl);
  Macro(std::string cs, std::vector<Token>&& param, std::vector<Token>&& repl);

  const std::string& controlSequence() const;
  const std::vector<Token>& parameterText() const;
  const std::vector<Token>& replacementText() const;

  Macro& operator=(const Macro&) = default;
  Macro& operator=(Macro&&) = default;

private:
  std::string m_ctrl_seq;
  std::vector<Token> m_param_text;
  std::vector<Token> m_repl_text;
};

class LIBTYPESET_API Preprocessor
{
public:
  explicit Preprocessor(Memory* mem = nullptr, InputStream* is = nullptr);
  ~Preprocessor() = default;

  struct Definitions
  {
    std::map<std::string, Macro> macros;
  };

  typedef std::array<std::vector<Token>, 9> Arguments;

  void beginGroup();
  void endGroup();

  void write(const Token& t);
  void write(Token&& t);

  bool advance();

  bool isDone() const;

  std::vector<Token>& output();



  struct MatchResult
  {
    enum ResultCode
    {
      PartialMatch,
      CompleteMatch,
      NoMatch,
    };

    ResultCode result = NoMatch;
    size_t begin = 0;
    size_t end = 0;
  };

  static MatchResult match(std::vector<Token>& input, const std::vector<Token>& pattern, Arguments& args);

  const Macro* find(const std::string& cs) const;

  bool processControlSeq();

protected:
  bool readMacroDef();

  bool expandMacro(const Macro& mdef, std::vector<Token>& toks, std::vector<Token>::iterator begin);
  bool expandMacro(const Macro& mdef, std::vector<Token>& toks);

private:
  Memory* m_memory = nullptr;
  InputStream* m_inputstream = nullptr;
  std::list<Definitions> m_defs;
  std::vector<Token> m_input;
  std::vector<Token> m_output;
};

} // namespace parsing

} // namespace tex

namespace tex
{

namespace parsing
{

inline Macro::Macro(std::string cs, std::vector<Token>&& repl)
  : m_ctrl_seq(std::move(cs)),
  m_repl_text(std::move(repl))
{

}

inline Macro::Macro(std::string cs, std::vector<Token>&& param, std::vector<Token>&& repl)
  : m_ctrl_seq(std::move(cs))
  , m_param_text(std::move(param))
  , m_repl_text(std::move(repl))
{

}

inline const std::string& Macro::controlSequence() const
{
  return m_ctrl_seq;
}

inline const std::vector<Token>& Macro::parameterText() const
{
  return m_param_text;
}

inline const std::vector<Token>& Macro::replacementText() const
{
  return m_repl_text;
}

inline Preprocessor::Preprocessor(Memory* mem, InputStream* is)
  : m_memory(mem),
  m_inputstream{is},
  m_defs{ Definitions{} }
{

}

inline void Preprocessor::beginGroup()
{
  m_defs.push_front(Definitions{});
}

inline void Preprocessor::endGroup()
{
  m_defs.pop_front();
}

inline void Preprocessor::write(const Token& t)
{
  write(Token{ t });
}

inline void Preprocessor::write(Token&& t)
{
  m_input.emplace_back(std::move(t));
}

inline bool Preprocessor::advance()
{
  if (m_input.empty())
  {
    return true;
  }

  if (peek(m_input).isCharacterToken())
  {
    parsing::write(read(m_input), m_output);
    return m_input.empty();
  }
  else if (peek(m_input).isControlSequence())
  {
    return processControlSeq();
  }
  else
  {
    throw std::runtime_error{ "Illegal parameter token in token stream" };
  }
}

inline bool Preprocessor::isDone() const
{
  return m_input.empty();
}

inline std::vector<Token>& Preprocessor::output()
{
  return m_output;
}

} // namespace parsing

} // namespace tex


#endif // LIBTYPESET_PARSING_PREPROCESSOR_H

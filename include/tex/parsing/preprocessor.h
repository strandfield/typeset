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

  struct MatchResult
  {
    enum ResultCode
    {
      PartialMatch,
      CompleteMatch,
      NoMatch,
    };

    ResultCode result = NoMatch;
    size_t size = 0;
    std::array<std::vector<Token>, 9> arguments;

    operator bool() const { return result == CompleteMatch; }
  };

  MatchResult match(const std::vector<Token>& text) const;

  std::vector<Token> expand(const std::array<std::vector<Token>, 9>& arguments) const;
  void expand(const std::array<std::vector<Token>, 9> & arguments, std::vector<Token>& output, std::vector<Token>::iterator output_it) const;

  Macro& operator=(const Macro&) = default;
  Macro& operator=(Macro&&) = default;

private:
  std::string m_ctrl_seq;
  std::vector<Token> m_param_text;
  std::vector<Token> m_repl_text;
};

} // namespace parsing

} // namespace tex

namespace tex
{

namespace parsing
{

struct Registers;

namespace preprocessor
{

struct MacroDefinitionData
{
  std::string csname;
  int parameter_index = 1;
  std::vector<Token> parameter_text;
  int brace_nesting = 0;
  std::vector<Token> replacement_text;
};

struct MacroExpansionData
{
  const Macro* def = nullptr;
  size_t pattern_index = 0;
  size_t current_arg_index = 0;
  int current_arg_brace_nesting = 0;
  std::array<std::vector<Token>, 9> arguments;
};

} // namespace preprocessor

class LIBTYPESET_API Preprocessor
{
public:
  Preprocessor() = delete;
  Preprocessor(const Preprocessor&) = delete;
  Preprocessor(Preprocessor&&) = delete;
  ~Preprocessor() = default;

  explicit Preprocessor(Registers& r);

  struct Definitions
  {
    std::map<std::string, Macro> macros;
  };

  typedef std::array<std::vector<Token>, 9> Arguments;

  void beginGroup();
  void endGroup();

  void write(const Token& t);
  void write(Token&& t);

  void advance();

  std::vector<Token>& input();
  const std::vector<Token>& input() const;
  std::vector<Token>& output();

  struct State
  {
    State() = default;
    State(const State&) = delete;
    State(State&&) = default;

    enum FrameType
    {
      Idle,
      ReadingMacro, // RM
      ExpandingMacro, // EXPM
    };

    enum FrameSubType
    {
      FST_None = 0,
      RM_ReadingMacroName,
      RM_ReadingMacroParameterText,
      RM_ReadingMacroReplacementText,
      EXPM_MatchingMacroParameterText,
      EXPM_ReadingDelimitedMacroArgument,
      EXPM_ReadingUndelimitedMacroArgument,
      EXPM_ReadingBracedDelimitedMacroArgument,
    };

    struct Frame
    {
      Frame(const Frame&) = delete;
      Frame(Frame&& f);
      ~Frame();

      explicit Frame(FrameType ft);

      FrameType type;
      FrameSubType subtype;

      union {
        preprocessor::MacroExpansionData* macro_expansion;
        preprocessor::MacroDefinitionData* macro_definition;
      };
    };

    std::vector<Frame> frames;
  };

  const State& state() const;

  const Macro* find(const std::string& cs) const;

  void processControlSeq();

  Preprocessor& operator=(const Preprocessor&) = delete;

protected:
  void enter(State::FrameType s);
  void leave();

  State::Frame& currentFrame();

  void readMacro();

  void expandMacro();
  void updateExpandMacroState();

private:
  Registers& m_registers;
  std::list<Definitions> m_defs;
  std::vector<Token> m_input;
  std::vector<Token> m_output;
  State m_state;
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

  if (m_input.size() == 1)
    advance();
}

inline std::vector<Token>& Preprocessor::input()
{
  return m_input;
}

inline const std::vector<Token>& Preprocessor::input() const
{
  return m_input;
}

inline std::vector<Token>& Preprocessor::output()
{
  return m_output;
}

inline const Preprocessor::State& Preprocessor::state() const
{
  return m_state;
}

} // namespace parsing

} // namespace tex

#endif // LIBTYPESET_PARSING_PREPROCESSOR_H

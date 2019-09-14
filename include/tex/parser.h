// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_PARSER_H
#define LIBTYPESET_PARSER_H

#include "tex/lexer.h"

#include "tex/parsing/typesetting-instructions.h"

#include <array>
#include <cassert>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

namespace tex
{

class Symbol;

namespace parsing
{

class ParserMachine
{
public:

  ParserMachine();

  typedef void(ParserMachine::*TransitionFunction)();

  enum class State
  {
    ParsingDocument,
    ParsingCommand,
    ParsingGroup,
    StateDone,
  };

  enum class Mode
  {
    VericalMode,
    HorizontalMode,
    MathMode,
  };

  struct StateData
  {
    State state = State::ParsingDocument;
    TransitionFunction transition = nullptr;
    std::vector<Token> tokens;
    std::vector<std::shared_ptr<tex::typesetting::TypesettingInstruction>> output;
  };

  State currentState() const { return m_state.back().state; }

  const std::vector<StateData>& state() const;

  void advance();

  void write(const Token& tok);

  void write(const std::vector<Token>& toks)
  {
    for (const Token& t: toks)
      write(t);
  }

  std::vector<std::shared_ptr<typesetting::TypesettingInstruction>>& output();

protected:
  std::vector<Token>& currentTokens();
  std::vector<std::shared_ptr<typesetting::TypesettingInstruction>>& currentOutput();

  void enter(TransitionFunction tf);
  void leave();

protected:
  void advanceParsingDocument();
  void advanceParsingGroup();

protected:
  void advanceCommandPar();

protected:
  void emitInstruction(const std::shared_ptr<typesetting::TypesettingInstruction>& instr);
  void emitTypesetInstruction(char c);
  void emitInterwordGlueInstruction();

protected:
  std::map<std::string, TransitionFunction> m_commands;

private:
  std::vector<StateData> m_state;
  std::vector<std::shared_ptr<tex::typesetting::TypesettingInstruction>> m_output;
};

//template<typename T, typename P>
//class DocumentProcessor
//{
//private:
//  T m_tokenizer;
//  P m_parser;
//
//public:
//
//  void write(char c)
//  {
//    m_tokenizer.write(c);
//
//    if (m_tokenizer.output().empty())
//      return;
//
//    auto& outputs = m_tokenizer.output();
//
//    for (const Token& tok : outputs)
//    {
//      m_parser.write(tok);
//    }
//
//    outputs.clear();
//
//    if (m_parser.output().empty())
//      return;
//
//    /// TODO: process parser output
//  }
//
//  void write(const std::string& chars)
//  {
//    for (char c : chars)
//      write(c);
//  }
//};

} // namespace parsing

} // namespace tex

namespace tex
{

namespace parsing
{

inline ParserMachine::ParserMachine()
{
  m_state.emplace_back();

  m_commands["par"] = &ParserMachine::advanceCommandPar;
}

inline const std::vector<ParserMachine::StateData>& ParserMachine::state() const
{
  return m_state;
}

inline std::vector<std::shared_ptr<typesetting::TypesettingInstruction>>& ParserMachine::output()
{
  return m_state.front().output;
}

inline std::vector<Token>& ParserMachine::currentTokens()
{
  return m_state.back().tokens;
}

inline std::vector<std::shared_ptr<typesetting::TypesettingInstruction>>& ParserMachine::currentOutput()
{
  return m_state.back().output;
}

inline void ParserMachine::enter(TransitionFunction tf)
{
  m_state.emplace_back();
  m_state.back().state = State::ParsingCommand;
  m_state.back().transition = tf;
}

inline void ParserMachine::leave()
{
  std::vector<std::shared_ptr<typesetting::TypesettingInstruction>> output{ std::move(m_state.back().output) };
  m_state.pop_back();
  m_state.back().output.insert(m_state.back().output.end(), output.begin(), output.end());
}

inline void ParserMachine::advance()
{
  if (currentState() == State::ParsingDocument)
  {
    advanceParsingDocument();
  }
  else if (currentState() == State::ParsingCommand)
  {
    TransitionFunction f = m_state.back().transition;
    (this->*f)();
  }
  else if (currentState() == State::ParsingGroup)
  {
    advanceParsingGroup();
  }
}

inline void ParserMachine::advanceParsingDocument()
{
  advanceParsingGroup();
}

inline void ParserMachine::advanceParsingGroup()
{
  if (currentTokens().front().isControlSequence())
  {
    auto it = m_commands.find(currentTokens().front().controlSequence());

    if (it == m_commands.end())
    {
      throw std::runtime_error{ "Unknown control sequence" };
    }

    enter(it->second);
    advance();
  }
  else
  {
    assert(currentTokens().size() == 1);

    CharacterToken ctok = currentTokens().front().characterToken();
    currentTokens().pop_back();

    switch (ctok.category)
    {
    case CharCategory::Letter:
    case CharCategory::Other:
      emitTypesetInstruction(ctok.value);
      break;
    case CharCategory::Space:
      emitInterwordGlueInstruction();
      break;
    case CharCategory::GroupBegin:
    {
      StateData new_state;
      new_state.state = State::ParsingGroup;
      m_state.push_back(new_state);
    }
    break;
    case CharCategory::GroupEnd:
    {
      std::vector<std::shared_ptr<typesetting::TypesettingInstruction>> instructions{ std::move(m_state.back().output) };
      m_state.pop_back();

      auto& output = m_state.back().output;
      output.push_back(std::make_shared<typesetting::EnterGroup>());
      output.insert(output.end(), instructions.begin(), instructions.end());
      output.push_back(std::make_shared<typesetting::LeaveGroup>());
    }
    break;
    case CharCategory::MathShift:
      // TODO
      break;
    case CharCategory::Ignored:
      break;
    case CharCategory::Parameter:
    case CharCategory::AlignmentTab:
      throw std::runtime_error{ "Unexpected character token" };
    default:
      break;
    }
  }
}

inline void ParserMachine::advanceCommandPar()
{
  emitInstruction(std::make_shared<typesetting::EndParagraph>());
  leave();
}

inline void ParserMachine::emitInstruction(const std::shared_ptr<typesetting::TypesettingInstruction>& instr)
{
  m_state.back().output.push_back(instr);
}

inline void ParserMachine::emitTypesetInstruction(char c)
{
  emitInstruction(std::make_shared<typesetting::TypesetText>(std::make_shared<TextSymbol>(std::string({ c }))));
}

inline void ParserMachine::emitInterwordGlueInstruction()
{
  emitInstruction(std::make_shared<typesetting::InsertInterwordGlue>());
}

inline void ParserMachine::write(const Token& tok)
{
  m_state.back().tokens.push_back(tok);
  advance();
}

} // namespace parsing

} // namespace tex

#endif // LIBTYPESET_PARSER_H

// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_LEXER_H
#define LIBTYPESET_LEXER_H

#include "tex/token.h"

#include <vector>

namespace tex
{

namespace parsing
{

enum class LexerState {
  StateN, // Beginning a new line
  StateM, // Middle of a line
  StateS, // Skipping blanks
  StateCS, // Control sequence
  StateP, // Reading parameter
  StateCOM, // Comments
};

class LIBTYPESET_API Lexer
{
public:

  struct State
  {
    std::array<CharCategory, 256> catcodes;
    LexerState state = LexerState::StateN;
  };

  typedef std::array<CharCategory, 256> CatCodeTable;

  static const CatCodeTable DefaultCatCodes;

private:
  State m_state;
  std::string m_csbuffer;
  std::vector<Token> m_tokens;

public:

  Lexer()
  {
   m_state.catcodes = DefaultCatCodes;
  }

  ~Lexer() = default;

  const LexerState& state() const { return m_state.state; }
  LexerState& state() { return m_state.state; }

  const std::array<CharCategory, 256> & catcodes() const { return m_state.catcodes; }
  std::array<CharCategory, 256>& catcodes() { return m_state.catcodes; }

  std::vector<Token>& output() { return m_tokens; }

  CharCategory category(char c) const { return catcodes().at(static_cast<unsigned char>(c)); }

  void write(char c);

protected:
  void parseCS(char c, CharCategory cc);
  void parseCOM(char c, CharCategory cc);
  void produce(char c, CharCategory cc);
  void produceCSToken();
  void produceParamToken(char c);
};

inline void Lexer::write(char c)
{
  LexerState& s = state();
    
  const CharCategory cc = category(c);

  if (s == LexerState::StateCS)
  {
    return parseCS(c, cc);
  }
  else if (s == LexerState::StateCOM)
  {
    return parseCOM(c, cc);
  }
  else if (s == LexerState::StateP)
  {
    if (cc == CharCategory::Parameter)
    {
      produce(c, cc);
    }
    else
    {
      produceParamToken(c);
    }

    s = LexerState::StateM;
    return;
  }

  switch (cc)
  {
  case CharCategory::Escape:
  {
    m_csbuffer.clear();
    s = LexerState::StateCS;
  }
  break;
  case CharCategory::Parameter:
  {
    s = LexerState::StateP;
  }
  break;
  case CharCategory::GroupBegin:
  case CharCategory::GroupEnd:
  case CharCategory::MathShift:
  case CharCategory::AlignmentTab:
  case CharCategory::Superscript:
  case CharCategory::Subscript:
  case CharCategory::Letter:
  case CharCategory::Other:
  case CharCategory::Active:
  {
    produce(c, cc);
    s = LexerState::StateM;
  }  
  break;
  case CharCategory::EndOfLine:
  {
    if (s == LexerState::StateN)
    {
      m_csbuffer = "par";
      produceCSToken();
    }
    else if (s == LexerState::StateM)
    {
      produce(' ', CharCategory::Space);
    }
    else if (s == LexerState::StateS)
    {
      /* no-op */
    }
    
    s = LexerState::StateN;
  }
    break;
  case CharCategory::Ignored:
    break;
  case CharCategory::Space:
  {
    if (s == LexerState::StateM)
    {
      produce(' ', CharCategory::Space);
      s = LexerState::StateS;
    }
  }
  break;
  case CharCategory::Comment:
  {
    s = LexerState::StateCOM;
  }
  break;
  case CharCategory::Invalid:
  {
    throw std::runtime_error{ "Lexer received an invalid character" };
  }
  break;
  default:
    break;
  }
}

inline void Lexer::parseCS(char c, CharCategory cc)
{
  if (cc == CharCategory::EndOfLine)
  {
    produceCSToken();
    state() = LexerState::StateM;
  }
  else if (cc != CharCategory::Letter)
  {
    if (m_csbuffer.empty())
    {
      m_csbuffer.push_back(c);
      produceCSToken();
      state() = cc == CharCategory::Space ? LexerState::StateS : LexerState::StateM;
    }
    else
    {
      produceCSToken();
      state() = LexerState::StateS;
      write(c);
    }
  }
  else
  {
    m_csbuffer.push_back(c);
  }
}

inline void Lexer::parseCOM(char /* c */, CharCategory cc)
{
  if (cc == CharCategory::EndOfLine)
  {
    state() = LexerState::StateN;
  }
}

inline void Lexer::produce(char c, CharCategory cc)
{
  m_tokens.push_back(Token{ CharacterToken{c, cc} });
}

inline void Lexer::produceCSToken()
{
  m_tokens.push_back(Token{ m_csbuffer });
}

inline void Lexer::produceParamToken(char c)
{
  if (c >= '1' && c <= '9')
  {
    m_tokens.push_back(Token{ static_cast<int>(c - '0') });
  }
  else
  {
    throw std::runtime_error{ "Invalid param token" };
  }
}

} // namespace parsing

} // namespace tex

#endif // LIBTYPESET_LEXER_H

// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_TOKEN_H
#define LIBTYPESET_TOKEN_H

#include "tex/defs.h"

#include <array>
#include <stdexcept>
#include <string>
#include <vector>

namespace tex
{

namespace parsing
{

enum class CharCategory {
  Escape = 0,
  GroupBegin = 1,
  GroupEnd = 2,
  MathShift = 3,
  AlignmentTab = 4,
  EndOfLine = 5,
  Parameter = 6,
  Superscript = 7,
  Subscript = 8,
  Ignored = 9,
  Space = 10,
  Letter = 11,
  Other = 12,
  Active = 13,
  Comment = 14,
  Invalid = 15,
};

enum class TokenType {
  CharacterToken,
  ControlSequenceToken,
};

struct CharacterToken
{
  char value = '\0';
  CharCategory category = CharCategory::Invalid;
};

class Token
{
private:

  // C++17: use std::variant
  union Data
  {
    CharacterToken character_token;
    std::string control_sequence;

    Data() { new (&character_token) CharacterToken;  }
    ~Data() { }
  };

  TokenType m_type = TokenType::CharacterToken;
  Data m_data;

public:
  Token() = default;
  Token(const Token& other);
  Token(Token&& other);
  ~Token();

  Token(const CharacterToken& ctok);
  explicit Token(const std::string& cseq);

  TokenType type() const { return m_type; }
  bool isControlSequence() const { return type() == TokenType::ControlSequenceToken; }
  bool isCharacterToken() const { return type() == TokenType::CharacterToken; }

  const CharacterToken& characterToken() const;
  const std::string& controlSequence() const;

  Token& operator=(const Token& other);
  Token& operator=(Token&& other);

};

inline Token::~Token()
{
  if (isControlSequence())
  {
    using Str = std::string;
    (&m_data.control_sequence)->~Str();
  }
  else
  {
    (&m_data.character_token)->~CharacterToken();
  }
}

inline Token::Token(const Token& other)
{
  if (other.isControlSequence())
  {
    m_type = TokenType::ControlSequenceToken;
    (&m_data.character_token)->~CharacterToken();
    new (&m_data.control_sequence) std::string(other.controlSequence());
  }
  else
  {
    m_data.character_token = other.characterToken();
  }
}

inline Token::Token(Token&& other)
{
  if (other.isControlSequence())
  {
    m_type = TokenType::ControlSequenceToken;
    (&m_data.character_token)->~CharacterToken();
    new (&m_data.control_sequence) std::string(std::move(other.m_data.control_sequence));
  }
  else
  {
    m_data.character_token = other.characterToken();
  }
}

inline Token::Token(const CharacterToken& ctok)
{
  m_data.character_token = ctok;
}

inline Token::Token(const std::string& cseq)
  : m_type(TokenType::ControlSequenceToken)
{
  (&m_data.character_token)->~CharacterToken();
  new (&m_data.control_sequence) std::string(cseq);
}

inline const CharacterToken& Token::characterToken() const 
{
  return m_data.character_token;
}

inline const std::string& Token::controlSequence() const
{
  return m_data.control_sequence;
}


inline Token& Token::operator=(const Token& other)
{
  if (isControlSequence())
  {
    if (other.isControlSequence())
    {
      m_data.control_sequence = other.controlSequence();
    }
    else
    {
      using Str = std::string;
      (&m_data.control_sequence)->~Str();
      new (&m_data.character_token) CharacterToken(other.characterToken());
      m_type = TokenType::CharacterToken;
    }
  }
  else
  {
    if (other.isControlSequence())
    {
      (&m_data.character_token)->~CharacterToken();
      new (&m_data.control_sequence) std::string(other.controlSequence());
      m_type = TokenType::ControlSequenceToken;
    }
    else
    {
      m_data.character_token = other.characterToken();
    }
  }

  return *this;
}

inline Token& Token::operator=(Token&& other)
{
  if (isControlSequence())
  {
    if (other.isControlSequence())
    {
      m_data.control_sequence = std::move(other.m_data.control_sequence);
    }
    else
    {
      using Str = std::string;
      (&m_data.control_sequence)->~Str();
      new (&m_data.character_token) CharacterToken(other.characterToken());
      m_type = TokenType::CharacterToken;
    }
  }
  else
  {
    if (other.isControlSequence())
    {
      (&m_data.character_token)->~CharacterToken();
      new (&m_data.control_sequence) std::string(std::move(other.m_data.control_sequence));
      m_type = TokenType::ControlSequenceToken;
    }
    else
    {
      m_data.character_token = other.characterToken();
    }
  }

  return *this;
}

inline bool operator==(const Token& lhs, const Token& rhs)
{
  return lhs.type() == rhs.type()
    && (lhs.type() == TokenType::CharacterToken ? lhs.characterToken() == rhs.characterToken() : true)
    && (lhs.type() == TokenType::ControlSequenceToken ? lhs.controlSequence() == rhs.controlSequence() : true);
}

inline bool operator!=(const Token& lhs, const Token& rhs)
{
  return !(lhs == rhs);
}

} // namespace parsing

} // namespace tex

#endif // LIBTYPESET_TOKEN_H

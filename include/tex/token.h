// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_TOKEN_H
#define LIBTYPESET_TOKEN_H

#include "tex/defs.h"

#include <array>
#include <stdexcept>
#include <string>

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
  ParameterToken,
};

struct CharacterToken
{
  char value = '\0';
  CharCategory category = CharCategory::Invalid;
};

inline bool operator==(const CharacterToken& lhs, const CharacterToken& rhs)
{
  return lhs.value == rhs.value && lhs.category == rhs.category;
}

inline bool operator!=(const CharacterToken& lhs, const CharacterToken& rhs)
{
  return !(lhs == rhs);
}

class LIBTYPESET_API Token
{
private:

  // C++17: use std::variant
  union Data
  {
    CharacterToken character_token;
    std::string control_sequence;
    int parameter_number;

    Data() { new (&character_token) CharacterToken;  }
    ~Data() { }
  };

  TokenType m_type = TokenType::CharacterToken;
  Data m_data;
  
  // TODO: avoid std::string, use string_view
  // 3 fields: string_view & TokenType & CharCategory
  // if tokentype is chartok or paramtok, string_view size is 1
  // use the constexpr char literal "123456789" & "abcdefghij" (all ascii chars)

public:
  Token() = default;
  Token(const Token& other);
  Token(Token&& other);
  ~Token();

  Token(const CharacterToken& ctok);
  explicit Token(const std::string& cseq);
  explicit Token(int param_num);

  TokenType type() const { return m_type; }
  bool isControlSequence() const { return type() == TokenType::ControlSequenceToken; }
  bool isCharacterToken() const { return type() == TokenType::CharacterToken; }
  bool isParameterToken() const { return type() == TokenType::ParameterToken; }

  const CharacterToken& characterToken() const;
  const std::string& controlSequence() const;
  int parameterNumber() const;

  Token& operator=(const Token& other);
  Token& operator=(Token&& other);

  bool operator==(CharCategory cc) const;
  bool operator!=(CharCategory cc) const;
};

inline Token::~Token()
{
  if (isControlSequence())
  {
    using Str = std::string;
    (&m_data.control_sequence)->~Str();
  }
  else if(isCharacterToken())
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
  else if (other.isParameterToken())
  {
    m_type = TokenType::ParameterToken;
    m_data.parameter_number = other.parameterNumber();
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
  else if (other.isParameterToken())
  {
    m_type = TokenType::ParameterToken;
    m_data.parameter_number = other.parameterNumber();
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

inline Token::Token(int param_num)
  : m_type(TokenType::ParameterToken)
{
  m_data.parameter_number = param_num;
}

inline const CharacterToken& Token::characterToken() const 
{
  return m_data.character_token;
}

inline const std::string& Token::controlSequence() const
{
  return m_data.control_sequence;
}

inline int Token::parameterNumber() const
{
  return m_data.parameter_number;
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

      if (other.isParameterToken())
      {
        m_data.parameter_number = other.parameterNumber();
        m_type = TokenType::ParameterToken;
      }
      else
      {
        new (&m_data.character_token) CharacterToken(other.characterToken());
        m_type = TokenType::CharacterToken;
      }
    }
  }
  else if (isParameterToken())
  {
    if (other.isParameterToken())
    {
      m_data.parameter_number = other.parameterNumber();
    }
    else
    {
      if (other.isControlSequence())
      {
        new (&m_data.control_sequence) std::string(other.controlSequence());
        m_type = TokenType::ControlSequenceToken;
      }
      else
      {
        new (&m_data.character_token) CharacterToken(other.characterToken());
        m_type = TokenType::CharacterToken;
      }
    }
  }
  else
  {
    if (other.isCharacterToken())
    {
      m_data.character_token = other.characterToken();
    }
    else
    {
      (&m_data.character_token)->~CharacterToken();

      if (other.isControlSequence())
      {
        new (&m_data.control_sequence) std::string(other.controlSequence());
        m_type = TokenType::ControlSequenceToken;
      }
      else
      {
        m_data.parameter_number = other.parameterNumber();
        m_type = TokenType::ParameterToken;
      }
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

      if (other.isParameterToken())
      {
        m_data.parameter_number = other.parameterNumber();
        m_type = TokenType::ParameterToken;
      }
      else
      {
        new (&m_data.character_token) CharacterToken(other.characterToken());
        m_type = TokenType::CharacterToken;
      }
    }
  }
  else if (isParameterToken())
  {
    if (other.isParameterToken())
    {
      m_data.parameter_number = other.parameterNumber();
    }
    else
    {
      if (other.isControlSequence())
      {
        new (&m_data.control_sequence) std::string(std::move(other.m_data.control_sequence));
        m_type = TokenType::ControlSequenceToken;
      }
      else
      {
        new (&m_data.character_token) CharacterToken(other.characterToken());
        m_type = TokenType::CharacterToken;
      }
    }
  }
  else
  {
    if (other.isCharacterToken())
    {
      m_data.character_token = other.characterToken();
    }
    else
    {
      (&m_data.character_token)->~CharacterToken();

      if (other.isControlSequence())
      {
        new (&m_data.control_sequence) std::string(std::move(other.m_data.control_sequence));
        m_type = TokenType::ControlSequenceToken;
      }
      else
      {
        m_data.parameter_number = other.parameterNumber();
        m_type = TokenType::ParameterToken;
      }
    }
  }

  return *this;
}

inline bool Token::operator==(CharCategory cc) const
{
  return isCharacterToken() && characterToken().category == cc;
}

inline bool Token::operator!=(CharCategory cc) const
{
  return !(*this == cc);
}

inline bool operator==(const Token& lhs, const Token& rhs)
{
  return lhs.type() == rhs.type()
    && (lhs.type() == TokenType::CharacterToken ? lhs.characterToken() == rhs.characterToken() : true)
    && (lhs.type() == TokenType::ControlSequenceToken ? lhs.controlSequence() == rhs.controlSequence() : true)
    && (lhs.type() == TokenType::ParameterToken ? lhs.parameterNumber() == rhs.parameterNumber() : true);
}

inline bool operator!=(const Token& lhs, const Token& rhs)
{
  return !(lhs == rhs);
}

} // namespace parsing

} // namespace tex

#endif // LIBTYPESET_TOKEN_H

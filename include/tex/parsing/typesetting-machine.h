// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_PARSING_TYPESETTINGMACHINE_H
#define LIBTYPESET_PARSING_TYPESETTINGMACHINE_H

#include "tex/parsing/inputstream.h"
#include "tex/parsing/preprocessor.h"
#include "tex/parsing/registers.h"
#include "tex/lexer.h"
#include "tex/parsing/mode.h"

#include "tex/typeset.h"

#include <memory>
#include <stack>
#include <vector>

namespace tex
{

namespace parsing
{

struct Memory
{
  std::array<int, 256> registers;
  std::array<std::vector<Token>, 256> toks;
};

class LIBTYPESET_API TypesettingMachine
{
public:
  TypesettingMachine(const TypesettingMachine&) = delete;
  ~TypesettingMachine() = default;

  TypesettingMachine(std::shared_ptr<TypesetEngine> te, const Options& opts);

  const std::shared_ptr<TypesetEngine>& typesetEngine() const;
  const Options& options() const;

  bool typesetIndividualCharacters() const;

  InputStream& inputStream();
  Lexer& lexer();
  Preprocessor& preprocessor();

  void advance();

  void beginGroup();
  void endGroup();

  const std::vector<std::unique_ptr<Mode>>& modes() const;
  Mode& currentMode() const;

  void enter(std::unique_ptr<Mode>&& m);
  void leave();

  template<typename T, typename...Args>
  void enter(Args&& ...);

  TypesettingMachine& operator=(const TypesettingMachine&) = delete;

private:
  Registers m_registers;
  Memory m_memory;
  std::stack<Lexer::CatCodeTable> m_lexercatcodes;
  InputStream m_inputstream;
  Lexer m_lexer;
  Preprocessor m_preprocessor;
  std::vector<std::unique_ptr<Mode>> m_modes;
  bool m_typeset_chars = false;
  std::shared_ptr<TypesetEngine> m_typeset_engine;
  std::stack<Options> m_opts;
};

class LIBTYPESET_API TextTypesetter
{
public:
  explicit TextTypesetter(std::shared_ptr<TypesetEngine> te, Font f = Font::Default);
  ~TextTypesetter() = default;

  Font font() const;
  void setFont(Font f);

  std::shared_ptr<Box> print(const std::string& text) const;

  std::string& buffer();
  const std::string& buffer() const;
  bool ready() const;
  
  std::shared_ptr<Box> print();

private:
  std::shared_ptr<TypesetEngine> m_typeset_engine;
  Font m_font;
  std::string m_buffer;
};

} // namespace parsing

} // namespace tex

namespace tex
{

namespace parsing
{

inline const std::shared_ptr<TypesetEngine>& TypesettingMachine::typesetEngine() const
{
  return m_typeset_engine;
}

inline const Options& TypesettingMachine::options() const
{
  return m_opts.top();
}

inline bool TypesettingMachine::typesetIndividualCharacters() const
{
  return m_typeset_chars;
}

inline InputStream& TypesettingMachine::inputStream()
{
  return m_inputstream;
}

inline Lexer& TypesettingMachine::lexer()
{
  return m_lexer;
}

inline Preprocessor& TypesettingMachine::preprocessor()
{
  return m_preprocessor;
}

inline void TypesettingMachine::advance()
{
  m_lexer.write(m_inputstream.read());

  if (m_lexer.output().empty())
  {
    return;
  }

  for (auto& t : m_lexer.output())
  {
    m_preprocessor.write(std::move(t));
  }

  m_lexer.output().clear();

  for (;;)
  {
    while (!m_preprocessor.output().empty())
    {
      Token t = parsing::read(m_preprocessor.output());

      m_modes.back()->write(std::move(t));

      if (m_modes.back()->done())
      {
        m_modes.pop_back();
      }
    }

    if (!m_preprocessor.input().empty())
    {
      m_preprocessor.advance();
    }
    else
    {
      break;
    }
  }
}

inline const std::vector<std::unique_ptr<Mode>>& TypesettingMachine::modes() const
{
  return m_modes;
}

inline Mode& TypesettingMachine::currentMode() const
{
  return *m_modes.back();
}

inline void TypesettingMachine::enter(std::unique_ptr<Mode>&& m)
{
  m_modes.push_back(std::move(m));
}

inline void TypesettingMachine::leave()
{
  m_modes.pop_back();
}

template<typename T, typename...Args>
inline void TypesettingMachine::enter(Args&& ... args)
{
  using MachineType = decltype(std::declval<T>().machine());
  enter(std::make_unique<T>(static_cast<MachineType&>(*this), std::forward<Args>(args)...));
}

} // namespace parsing

} // namespace tex

#endif // LIBTYPESET_PARSING_TYPESETTINGMACHINE_H

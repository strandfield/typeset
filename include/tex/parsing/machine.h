// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_PARSING_MACHINE_H
#define LIBTYPESET_PARSING_MACHINE_H

#include "tex/parsing/inputstream.h"
#include "tex/parsing/preprocessor.h"
#include "tex/lexer.h"
#include "tex/parsing/mode.h"

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

class LIBTYPESET_API Machine
{
public:
  Machine();
  Machine(const Machine& ) = delete;
  ~Machine() = default;

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
  void enter(Args &&...);

  Machine& operator=(const Machine&) = delete;

private:
  Memory m_memory;
  std::stack<Lexer::CatCodeTable> m_lexercatcodes;
  InputStream m_inputstream;
  Lexer m_lexer;
  Preprocessor m_preprocessor;
  std::vector<std::unique_ptr<Mode>> m_modes;
};

} // namespace parsing

} // namespace tex

namespace tex
{

namespace parsing
{

inline Machine::Machine()
  : m_memory{},
    m_inputstream{},
    m_preprocessor{ &m_memory, &m_inputstream }
{

}

inline InputStream& Machine::inputStream()
{
  return m_inputstream;
}

inline Lexer& Machine::lexer()
{
  return m_lexer;
}

inline Preprocessor& Machine::preprocessor()
{
  return m_preprocessor;
}

inline void Machine::advance()
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

  RetCode rc = m_preprocessor.advance();

  while (rc != RetCode::Await)
  {
    rc = m_preprocessor.advance();
  }

  m_lexer.output().clear();

  if (m_preprocessor.output().empty())
  {
    return;
  }

  rc = m_modes.back()->advance();

  while (rc == RetCode::Yield && !m_preprocessor.output().empty())
  {
    rc = m_modes.back()->advance();
  }

  if (rc == RetCode::Return)
  {
    m_modes.pop_back();
  }
}

inline void Machine::beginGroup()
{
  m_preprocessor.beginGroup();
  m_lexercatcodes.push(m_lexer.catcodes());
}

inline void Machine::endGroup()
{
  m_lexer.catcodes() = m_lexercatcodes.top();
  m_lexercatcodes.pop();
  m_preprocessor.endGroup();
}

inline const std::vector<std::unique_ptr<Mode>>& Machine::modes() const
{
  return m_modes;
}

inline Mode& Machine::currentMode() const
{
  return *m_modes.back();
}

inline void Machine::enter(std::unique_ptr<Mode>&& m)
{
  m_modes.push_back(std::move(m));
}

inline void Machine::leave()
{
  m_modes.pop_back();
}

template<typename T, typename...Args>
inline void Machine::enter(Args&& ... args)
{
  using MachineType = decltype(std::declval<T>().machine());
  enter(std::make_unique<T>(static_cast<MachineType&>(*this), std::forward<Args>(args)...));
}

} // namespace parsing

} // namespace tex

#endif // LIBTYPESET_PARSING_MACHINE_H

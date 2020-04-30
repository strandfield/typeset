// Copyright (C) 2019-2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef TYPESET_PAGEEDITOR_TYPESETTINGMACHINE_H
#define TYPESET_PAGEEDITOR_TYPESETTINGMACHINE_H

#include "assignment-processor.h"
#include "inputstream.h"
#include "mode.h"

#include "common/qt-typeset-engine.h"

#include "tex/parsing/preprocessor.h"
#include "tex/parsing/registers.h"
#include "tex/lexer.h"

#include "tex/parshape.h"
#include "tex/typeset.h"
#include "tex/units.h"
#include "tex/vbox.h"

#include <memory>
#include <vector>

struct TypesettingMachineMemory
{
  tex::parsing::Lexer::CatCodeTable catcodes;
  tex::Font font;
  float prevdepth = -10000.f;
  std::shared_ptr<tex::Glue> baselineskip;
  std::shared_ptr<tex::Glue> lineskip;
  float lineskiplimit = 0.f;
  float hsize;
  tex::Parshape parshape;
};

class TypesettingException : public std::runtime_error
{
public:
  size_t line;
  size_t col;

  TypesettingException(size_t l, size_t c, const char* mssg);
};

class TypesettingMachine
{
public:
  TypesettingMachine(const TypesettingMachine&) = delete;
  ~TypesettingMachine() = default;

  TypesettingMachine(std::shared_ptr<TypesetEngine> te, tex::Font f);

  enum State
  {
    Idle,
    ReadChar,
    ReadToken,
    Preprocess,
    SendToken,
    DigestToken,
  };

  State state() const;

  std::shared_ptr<tex::VBox> typeset(std::string text);

  const std::shared_ptr<TypesetEngine>& typesetEngine() const;

  typedef TypesettingMachineMemory Memory;
  Memory& memory();
  const Memory& memory() const;

  InputStream& inputStream();
  tex::parsing::Lexer& lexer();
  tex::parsing::Preprocessor& preprocessor();

  tex::UnitSystem unitSystem() const;

  void resume();

  void beginGroup();
  void endGroup();

  const std::vector<std::unique_ptr<Mode>>& modes() const;
  Mode& currentMode() const;

  void insert(tex::parsing::Token&& tok);

  void enter(std::unique_ptr<Mode>&& m);
  void leaveCurrentMode();

  template<typename T, typename...Args>
  void enter(Args&& ...);

  TypesettingMachine& operator=(const TypesettingMachine&) = delete;

protected:
  void advance();
  void sendToken();
  bool digestToken();

private:
  tex::parsing::Registers m_registers;
  std::vector<Memory> m_memory;
  InputStream m_inputstream;
  tex::parsing::Lexer m_lexer;
  tex::parsing::Preprocessor m_preprocessor;
  AssignmentProcessor m_assignment_processor;
  std::vector<tex::parsing::Token> m_tokens;
  std::vector<std::unique_ptr<Mode>> m_modes;
  bool m_leave_current_mode = false;
  std::shared_ptr<TypesetEngine> m_typeset_engine;
  State m_state = State::Idle;
};

inline TypesettingMachine::State TypesettingMachine::state() const
{
  return m_state;
}

inline const std::shared_ptr<TypesetEngine>& TypesettingMachine::typesetEngine() const
{
  return m_typeset_engine;
}

inline TypesettingMachine::Memory& TypesettingMachine::memory()
{
  return m_memory.back();
}

inline const TypesettingMachine::Memory& TypesettingMachine::memory() const
{
  return m_memory.back();
}

inline InputStream& TypesettingMachine::inputStream()
{
  return m_inputstream;
}

inline tex::parsing::Lexer& TypesettingMachine::lexer()
{
  return m_lexer;
}

inline tex::parsing::Preprocessor& TypesettingMachine::preprocessor()
{
  return m_preprocessor;
}

inline const std::vector<std::unique_ptr<Mode>>& TypesettingMachine::modes() const
{
  return m_modes;
}

inline Mode& TypesettingMachine::currentMode() const
{
  return *m_modes.back();
}

template<typename T, typename...Args>
inline void TypesettingMachine::enter(Args&& ... args)
{
  using MachineType = decltype(std::declval<T>().machine());
  enter(std::make_unique<T>(static_cast<MachineType&>(*this), std::forward<Args>(args)...));
}

#endif // TYPESET_PAGEEDITOR_TYPESETTINGMACHINE_H

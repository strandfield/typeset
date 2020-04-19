// Copyright (C) 2019-2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "typesetting-machine.h"

#include "verticalmode.h"

#include "tex/unicode.h"

#include <cassert>

TypesettingException::TypesettingException(size_t l, size_t c, const char* mssg)
  : std::runtime_error(mssg),
    line(l),
    col(c)
{

}

TypesettingMachine::TypesettingMachine(std::shared_ptr<tex::TypesetEngine> te, tex::Font f)
  : m_memory{},
  m_inputstream{},
  m_preprocessor{ m_registers },
  m_typeset_engine(te)
{
  m_memory.emplace_back();
  memory().font = f;
  memory().catcodes = m_lexer.catcodes();
  memory().hsize = 800.f;

  tex::BoxMetrics metrics = te->metrics()->metrics('(', tex::Font(0));

  memory().baselineskip = tex::glue(1.2f * (metrics.height + metrics.depth));
  memory().lineskip = tex::glue(0.1f * (metrics.height + metrics.depth));

  enter<VerticalMode>();
}

std::shared_ptr<tex::VBox> TypesettingMachine::typeset(std::string text)
{
  m_inputstream = InputStream(std::move(text));

  m_state = State::ReadChar;
  resume();

  VerticalMode& vm = dynamic_cast<VerticalMode&>(currentMode());
  tex::List vlist = vm.vlist().result;
  return tex::vbox(std::move(vlist));
}

void TypesettingMachine::beginGroup()
{
  m_preprocessor.beginGroup();
  m_memory.emplace_back();
  m_memory.back() = m_memory.at(m_memory.size() - 2);
}

void TypesettingMachine::endGroup()
{
  m_memory.pop_back();
  m_preprocessor.endGroup();
}

void TypesettingMachine::insert(tex::parsing::Token&& tok)
{
  std::vector<tex::parsing::Token>& preproc_output = preprocessor().output();
  preproc_output.insert(preproc_output.begin(), std::move(tok));
}

void TypesettingMachine::enter(std::unique_ptr<Mode>&& m)
{
  m_modes.push_back(std::move(m));
}

void TypesettingMachine::leaveCurrentMode()
{
  m_leave_current_mode = true;
}

bool TypesettingMachine::sendTokens()
{
  if (m_preprocessor.output().empty())
    return false;

  tex::parsing::Token t = tex::parsing::read(m_preprocessor.output());

  currentMode().write(t);

  if (m_leave_current_mode)
  {
    m_leave_current_mode = false;
    m_modes.pop_back();
  }

  return !m_preprocessor.output().empty();
}

void TypesettingMachine::resume()
{
  while (state() != TypesettingMachine::Idle)
  {
    advance();
  }
}

void TypesettingMachine::advance()
{
  try
  {
    switch (state())
    {
    case State::ReadChar:
    {
      if (inputStream().atEnd())
      {
        while (m_modes.size() > 1)
        {
          currentMode().finish();
          m_leave_current_mode = false;
          m_modes.pop_back();
        }

        currentMode().finish();
        m_state = State::Idle;
      }
      else
      {
        m_lexer.write(inputStream().read());
        m_state = m_lexer.output().empty() ? State::ReadChar : State::ReadToken;
      }
    }
    break;
    case State::ReadToken:
    {
      if (!m_lexer.output().empty())
      {
        m_preprocessor.write(tex::parsing::read(m_lexer.output()));
        m_state = State::SendToken;
      }
      else
      {
        m_state = State::ReadChar;
      }
    }
    break;
    case State::Preprocess:
    {
      m_preprocessor.advance();

      if (!m_preprocessor.output().empty())
        m_state = State::SendToken;
      else if (!m_preprocessor.input().empty())
        m_state = State::Preprocess;
      else
        m_state = State::ReadChar;
    }
    break;
    case State::SendToken:
    {
      while (sendTokens());

      if (!m_preprocessor.input().empty())
        m_state = State::Preprocess;
      else
        m_state = State::ReadToken;
    }
    break;
    default:
      break;
    }
  }
  catch (std::runtime_error& ex)
  {
    auto pos = m_inputstream.position();
    throw TypesettingException{ pos.first, pos.second, ex.what() };
  }
}

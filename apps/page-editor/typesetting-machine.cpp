// Copyright (C) 2019-2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "typesetting-machine.h"

#include "pageeditor-format.h"
#include "verticalmode.h"

#include "tex/unicode.h"

#include <cassert>

template<typename C>
void send_token(tex::parsing::Token&& tok, std::vector<tex::parsing::Token>& output, C& c)
{
  c.write(std::move(tok));
  
  if (!c.output().empty())
  {
    output.insert(output.end(), c.output().begin(), c.output().end());
    c.output().clear();
  }
}

template<typename C, typename...Components, typename = std::enable_if_t<sizeof...(Components) != 0, void>>
void send_token(tex::parsing::Token&& tok, std::vector<tex::parsing::Token>& output, C& c, Components& ... rest)
{
  c.write(std::move(tok));

  if (!c.output().empty())
  {
    send_token(tex::parsing::read(c.output()), output, rest...);
  }
}


TypesettingException::TypesettingException(size_t l, size_t c, const char* mssg)
  : std::runtime_error(mssg),
    line(l),
    col(c)
{

}

TypesettingMachine::TypesettingMachine(std::shared_ptr<TypesetEngine> te, tex::Font f)
  : m_memory{},
  m_inputstream{},
  m_preprocessor{},
  m_assignment_processor{*this},
  m_typeset_engine(te)
{
  m_memory.emplace_back();
  memory().font = f;
  memory().catcodes = m_lexer.catcodes();
  memory().hsize = 800.f;

  tex::BoxMetrics metrics = te->metrics()->metrics('(', tex::Font(0));

  memory().baselineskip = tex::glue(1.2f * (metrics.height + metrics.depth));
  memory().lineskip = tex::glue(0.1f * (metrics.height + metrics.depth));

  for (const tex::parsing::Macro m : tex::parsing::Format::load(pageeditor_format()))
  {
    m_preprocessor.define(m);
  }

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

tex::UnitSystem TypesettingMachine::unitSystem() const
{
  tex::UnitSystem us;
  us.em = typesetEngine()->metrics()->quad(memory().font);
  us.ex = typesetEngine()->metrics()->xHeight(memory().font);
  tex::BoxMetrics bm = typesetEngine()->metrics()->metrics('(', memory().font);
  us.pt = (bm.depth + bm.height) / 10.f;
  return us;
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
  m_tokens.insert(m_tokens.begin(), std::move(tok));
}

void TypesettingMachine::enter(std::unique_ptr<Mode>&& m)
{
  m_modes.push_back(std::move(m));
}

void TypesettingMachine::leaveCurrentMode()
{
  m_leave_current_mode = true;
}

void TypesettingMachine::sendToken()
{
  if (m_preprocessor.output().empty())
    return;

  tex::parsing::Token t = tex::parsing::read(m_preprocessor.output());

  send_token(std::move(t), m_tokens, m_assignment_processor);
}

bool TypesettingMachine::digestToken()
{ 
  if (m_tokens.empty())
    return false;

  tex::parsing::Token t = tex::parsing::read(m_tokens);

  currentMode().write(t);

  if (m_leave_current_mode)
  {
    m_leave_current_mode = false;
    m_modes.pop_back();
  }

  return !m_tokens.empty();
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
      sendToken();
      m_state = State::DigestToken;
    }
    break;
    case State::DigestToken:
    {
      while (digestToken());

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

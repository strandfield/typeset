// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_PARSING_COMMANDS_H
#define LIBTYPESET_PARSING_COMMANDS_H

#include "tex/defs.h"

#include "tex/token.h"
#include "tex/parsing/retcode.h"

#include <map>
#include <stack>

namespace tex
{

namespace parsing
{

class Machine;
class Mode;

template<typename T = Mode>
struct Commands
{
  typedef bool(*EnterFunction)(Machine&, T&);
  typedef bool(*AdvanceFunction)(Machine&, T&);
  typedef void(*LeaveFunction)(Machine&, T&);

  struct CommandCallbacks
  {
    EnterFunction enter = nullptr;
    AdvanceFunction advance = nullptr;
    LeaveFunction leave = nullptr;
  };
  
  Machine& machine;
  T& mode;
  std::map<std::string, CommandCallbacks> m_commands;
  std::stack<CommandCallbacks> stack;

  Commands(Machine& ma, T& mo)
    : machine(ma),
    mode(mo)
  {

  }

  std::map<std::string, CommandCallbacks>& commands()
  {
    return m_commands;
  }

  bool isActive()
  {
    return !stack.empty();
  }

  void advance()
  {
    if (stack.top().advance(machine, mode))
    {
      stack.top().leave(machine, mode);
      stack.pop();
    }
  }

  void invoke(const CommandCallbacks& cmd)
  {
    stack.push(cmd);
    
    if (cmd.enter(machine, mode))
    {
      cmd.leave(machine, mode);
      stack.pop();
    }
  }
};

template<typename T = Mode, typename M = Machine>
struct CallStack
{
  typedef RetCode(*EnterFunction)(M&, T&);
  typedef RetCode(*AdvanceFunction)(M&, T&);
  typedef void(*LeaveFunction)(M&, T&);

  struct Frame
  {
    EnterFunction enter = nullptr;
    AdvanceFunction advance = nullptr;
    LeaveFunction leave = nullptr;
  };

  M& machine;
  T& mode;
  std::stack<Frame> stack;

  CallStack(M& ma, T& mo, Frame mainframe)
    : machine(ma),
    mode(mo)
  {
    stack.push(mainframe);
  }

  RetCode advance()
  {
    RetCode rc = stack.top().advance(machine, mode);

    if (rc == RetCode::Return)
    {
      if(stack.top().leave)
        stack.top().leave(machine, mode);

      stack.pop();
      return RetCode::Yield;
    }
    else
    {
      return rc;
    }
  }

  void push(const Frame& f)
  {
    stack.push(f);

    RetCode rc = f.enter(machine, mode);

    if (rc == RetCode::Return)
    {
      if(f.leave)
        f.leave(machine, mode);

      stack.pop();
      return RetCode::Yield;
    }

    return rc;
  }
};

} // namespace parsing

} // namespace tex

#endif // LIBTYPESET_PARSING_COMMANDS_H

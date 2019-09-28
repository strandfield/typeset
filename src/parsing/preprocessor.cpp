// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/parsing/preprocessor.h"

#include <cassert>

namespace tex
{

namespace parsing
{

struct Matcher
{
  std::vector<Token>& input;
  const std::vector<Token>& pattern;
  Preprocessor::Arguments& args;

  size_t input_index = 0;
  size_t pattern_index = 0;

  Preprocessor::MatchResult::ResultCode read_undelimited_arg()
  {
    const int param_num = pattern.at(pattern_index++).parameterNumber();

    if (input.at(input_index).isCharacterToken() && input.at(input_index).characterToken().category == CharCategory::Space)
    {
      ++input_index;

      if (input_index == input.size())
      {
        return Preprocessor::MatchResult::PartialMatch;
      }
    }

    if (input.at(input_index).isControlSequence())
    {
      args[param_num] = { input.at(input_index++) };
      return Preprocessor::MatchResult::CompleteMatch;
    }

    assert(input.at(input_index).isCharacterToken());

    if (input.at(input_index).characterToken().category == CharCategory::GroupBegin)
    {
      int brace_depth = 1;
      const size_t start_index = input_index;

      ++input_index;

      while (input_index < input.size() && brace_depth != 0)
      {
        if (input.at(input_index).isCharacterToken())
        {
          if (input.at(input_index).characterToken().category == CharCategory::GroupBegin)
          {
            brace_depth += 1;
          }
          else if (input.at(input_index).characterToken().category == CharCategory::GroupEnd)
          {
            brace_depth -= 1;
          }
        }

        ++input_index;
      }

      if (brace_depth != 0)
      {
        return Preprocessor::MatchResult::PartialMatch;
      }

      args[param_num] = std::vector<Token>(input.begin() + start_index + 1, input.begin() + input_index - 1);
      return Preprocessor::MatchResult::CompleteMatch;
    }
    else
    {
      args[param_num] = { input.at(input_index++) };
      return Preprocessor::MatchResult::CompleteMatch;
    }
  }

  Preprocessor::MatchResult::ResultCode read_delimited_arg()
  {
    const int param_num = pattern.at(pattern_index++).parameterNumber();
    const Token delimiter = pattern.at(pattern_index);

    int brace_depth = 0;
    const size_t start_index = input_index;

    while (input_index < input.size() && (input.at(input_index) != delimiter  || brace_depth != 0))
    {
      if (input.at(input_index).isCharacterToken())
      {
        if (input.at(input_index).characterToken().category == CharCategory::GroupBegin)
        {
          brace_depth++;
        }
        else if (input.at(input_index).characterToken().category == CharCategory::GroupEnd)
        {
          brace_depth--;
        }
      }
      else if (input.at(input_index).isControlSequence())
      {
        if (input.at(input_index).controlSequence() == "par")
        {
          return Preprocessor::MatchResult::NoMatch;
        }
      }

      ++input_index;
    }

    if (input_index == input.size())
    {
      return Preprocessor::MatchResult::PartialMatch;
    }

    args[param_num] = std::vector<Token>(input.begin() + start_index + 1, input.begin() + input_index - 1);
    return Preprocessor::MatchResult::CompleteMatch;
  }

  Preprocessor::MatchResult::ResultCode read_arg()
  {
    if (pattern_index == pattern.size() - 1 || pattern.at(pattern_index + 1).isParameterToken())
    {
      return read_undelimited_arg();
    }
    else
    {
      return read_delimited_arg();
    }
  }

  Preprocessor::MatchResult match()
  {
    Preprocessor::MatchResult r;
    r.begin = input_index;

    while (pattern_index < pattern.size())
    {
      if (input_index == input.size())
      {
        r.result = Preprocessor::MatchResult::PartialMatch;
        return r;
      }

      const Token& pat_tok = pattern.at(pattern_index);

      if (pat_tok.isCharacterToken() || pat_tok.isControlSequence())
      {
        if (input.at(input_index) != pattern.at(pattern_index))
        {
          r.result = Preprocessor::MatchResult::NoMatch;
          return r;
        }

        ++input_index;
        ++pattern_index;
      }
      else
      {
        r.result = read_arg();

        if (r.result != Preprocessor::MatchResult::CompleteMatch)
        {
          return r;
        }
      }
    }

    r.end = input_index;
    return r;
  }
};

Preprocessor::MatchResult Preprocessor::match(std::vector<Token>& input, const std::vector<Token>& pattern, Arguments& args)
{
  Matcher m{ input, pattern, args };
  m.input_index = 1;
  return m.match();
}

const Macro* Preprocessor::find(const std::string& cs) const
{
  for (const auto& defscope : m_defs)
  {
    auto it = defscope.macros.find(cs);

    if (it != defscope.macros.end())
    {
      return &(it->second);
    }
  }

  return nullptr;
}

bool Preprocessor::processControlSeq()
{
  Token cs = peek(m_input);

  if (cs.controlSequence() == "def")
  {
    return readMacroDef();
  }
  else
  {
    const Macro* m = find(cs.controlSequence());

    if (m == nullptr)
    {
      m_output.push_back(read(m_input));
      return true;
    }
    else
    {
      return expandMacro(*m, m_input);
    }
  }
}

bool Preprocessor::readMacroDef()
{
  if (m_input.size() < 4)
  {
    return false;
  }

  Token csname = m_input.at(1);

  const size_t pattern_begin = 2;
  size_t pattern_end = 2;

  /* Read pattern */

  while (pattern_end < m_input.size() && !(m_input.at(pattern_end) == CharCategory::GroupBegin))
  {
    ++pattern_end;
  }

  if (pattern_end == m_input.size())
  {
    return false;
  }

  /* Read replacement text */

  const size_t repl_begin = pattern_end;
  size_t repl_end = repl_begin + 1;
  int brace_depth = 0;

  while (repl_end < m_input.size() && (m_input.at(repl_end) != CharCategory::GroupEnd || brace_depth != 0))
  {
    if (m_input.at(repl_end) == CharCategory::GroupBegin)
    {
      ++brace_depth;
    }
    else if (m_input.at(repl_end) == CharCategory::GroupEnd)
    {
      --brace_depth;
    }

    ++repl_end;
  }

  if (repl_end == m_input.size())
  {
    return false;
  }

  Macro mdef{ csname.controlSequence(), std::vector<Token>(m_input.begin() + pattern_begin, m_input.begin() + pattern_begin),
    std::vector<Token>(m_input.begin() + repl_begin + 1, m_input.begin() + repl_end) };

  m_defs.front().macros[mdef.controlSequence()] = std::move(mdef);

  parsing::read(m_input, repl_end + 1);
  
  return true;
}

bool Preprocessor::expandMacro(const Macro& mdef, std::vector<Token>& toks, std::vector<Token>::iterator begin)
{
  Arguments args;

  MatchResult mr = match(toks, mdef.parameterText(), args);

  if (mr.result == MatchResult::NoMatch)
  {
    throw std::runtime_error{ "Use of macro does not match its definition" };
  }
  else if (mr.result == MatchResult::PartialMatch)
  {
    return false;
  }
 
  std::vector<Token> repl = mdef.replacementText();

  for (auto it = repl.begin(); it != repl.end();)
  {
    if (it->isParameterToken())
    {
      const int param_num = it->parameterNumber();

      it = repl.erase(it);
      it = repl.insert(it, args[param_num].begin(), args[param_num].end());
      it += args[param_num].size();
    }
    else
    {
      ++it;
    }
  }

  auto it = toks.erase(begin, toks.begin() + mr.end);
  toks.insert(it, repl.begin(), repl.end());

  return true;
}

bool Preprocessor::expandMacro(const Macro& mdef, std::vector<Token>& toks)
{
  return expandMacro(mdef, toks, toks.begin());
}

} // namespace parsing

} // namespace tex

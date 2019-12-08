// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/parsing/preprocessor.h"

#include "tex/parsing/registers.h"

#include <cassert>
#include <numeric>

namespace tex
{

namespace parsing
{

struct Matcher
{
  const std::vector<Token>& input;
  const std::vector<Token>& pattern;
  Macro::MatchResult result;

  size_t pattern_index = 0;

  bool read_undelimited_arg()
  {
    const int param_num = pattern.at(pattern_index++).parameterNumber() - 1;

    if (input.at(result.size).isCharacterToken() && input.at(result.size).characterToken().category == CharCategory::Space)
    {
      ++result.size;

      if (result.size == input.size())
      {
        result.result = Macro::MatchResult::PartialMatch;
        return false;
      }
    }

    if (input.at(result.size).isControlSequence())
    {
      result.arguments[param_num] = { input.at(result.size++) };
      return true;
    }

    assert(input.at(result.size).isCharacterToken());

    if (input.at(result.size).characterToken().category == CharCategory::GroupBegin)
    {
      int brace_depth = 1;
      const size_t start_index = result.size;

      ++result.size;

      while (result.size < input.size() && brace_depth != 0)
      {
        if (input.at(result.size).isCharacterToken())
        {
          if (input.at(result.size).characterToken().category == CharCategory::GroupBegin)
          {
            brace_depth += 1;
          }
          else if (input.at(result.size).characterToken().category == CharCategory::GroupEnd)
          {
            brace_depth -= 1;
          }
        }

        ++result.size;
      }

      if (brace_depth != 0)
      {
        result.result = Macro::MatchResult::PartialMatch;
        return false;
      }

      result.arguments[param_num] = std::vector<Token>(input.begin() + start_index + 1, input.begin() + result.size - 1);
      return true;
    }
    else
    {
      result.arguments[param_num] = { input.at(result.size++) };
      return true;
    }
  }

  bool read_delimited_arg()
  {
    const int param_num = pattern.at(pattern_index++).parameterNumber() - 1;
    const Token delimiter = pattern.at(pattern_index);

    int brace_depth = 0;
    const size_t start_index = result.size;

    while (result.size < input.size() && (input.at(result.size) != delimiter || brace_depth != 0))
    {
      if (input.at(result.size).isCharacterToken())
      {
        if (input.at(result.size).characterToken().category == CharCategory::GroupBegin)
        {
          brace_depth++;
        }
        else if (input.at(result.size).characterToken().category == CharCategory::GroupEnd)
        {
          brace_depth--;
        }
      }
      else if (input.at(result.size).isControlSequence())
      {
        if (input.at(result.size).controlSequence() == "par")
        {
          result.result = Macro::MatchResult::NoMatch;
          return false;
        }
      }

      ++result.size;
    }

    if (result.size == input.size())
    {
      result.result = Macro::MatchResult::PartialMatch;
      return false;
    }

    result.arguments[param_num] = std::vector<Token>(input.begin() + start_index, input.begin() + result.size);
    return true;
  }

  bool read_arg()
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

  Macro::MatchResult match()
  {
    while (pattern_index < pattern.size())
    {
      if (result.size == input.size())
      {
        result.result = Macro::MatchResult::PartialMatch;
        return result;
      }

      const Token& pat_tok = pattern.at(pattern_index);

      if (pat_tok.isCharacterToken() || pat_tok.isControlSequence())
      {
        if (input.at(result.size) != pattern.at(pattern_index))
        {
          result.result = Macro::MatchResult::NoMatch;
          return result;
        }

        ++result.size;
        ++pattern_index;
      }
      else
      {
        if (!read_arg())
        {
          return result;
        }
      }
    }

    result.result = Macro::MatchResult::CompleteMatch;
    return result;
  }
};

Macro::MatchResult Macro::match(const std::vector<Token>& text) const
{
  Matcher matcher{ text, parameterText() };
  return matcher.match();
}

std::vector<Token> Macro::expand(const std::array<std::vector<Token>, 9>& arguments) const
{
  std::vector<Token> result;
  result.reserve(replacementText().size() + std::accumulate(arguments.begin(), arguments.end(), 0, [](size_t n, const std::vector<Token>& arg) -> size_t {
    return n + arg.size();
    }));

  expand(arguments, result, result.end());

  return result;
}

void Macro::expand(const std::array<std::vector<Token>, 9>& arguments, std::vector<Token>& output, std::vector<Token>::iterator output_it) const
{
  std::vector<Token> repl = replacementText();

  for (auto it = repl.begin(); it != repl.end();)
  {
    if (it->isParameterToken())
    {
      const int param_num = it->parameterNumber();

      it = repl.erase(it);
      it = repl.insert(it, arguments.at(param_num-1).begin(), arguments.at(param_num - 1).end());
      it += arguments.at(param_num - 1).size();
    }
    else
    {
      ++it;
    }
  }

  output.insert(output_it, repl.begin(), repl.end());
}

Preprocessor::State::Frame::Frame(Frame&& f)
  : type(f.type),
    subtype(f.subtype)
{
  switch (f.type)
  {
  case ReadingMacro:
    macro_definition = f.macro_definition;
    f.macro_definition = nullptr;
    break;
  case ExpandingMacro:
    macro_expansion = f.macro_expansion;
    f.macro_expansion = nullptr;
    break;
  case Branching:
    branching = f.branching;
    f.branching = nullptr;
    break;
  default: break;
  }

  f.type = State::Idle;
  f.subtype = State::FST_None;
}

Preprocessor::State::Frame::Frame(Preprocessor::State::FrameType ft)
  : type(ft),
    subtype(FST_None)
{
  switch (ft)
  {
  case ReadingMacro:
    subtype = RM_ReadingMacroName;
    macro_definition = new preprocessor::MacroDefinitionData;
    break;
  case ExpandingMacro:
    subtype = EXPM_MatchingMacroParameterText;
    macro_expansion = new preprocessor::MacroExpansionData;
    break;
  case Branching:
    branching = new preprocessor::Branching;
    break;
  default: macro_definition = nullptr; break;
  }
}

Preprocessor::State::Frame::~Frame()
{
  switch (type)
  {
  case ReadingMacro:
    delete macro_definition;
    break;
  case ExpandingMacro:
    delete macro_expansion;
    break;
  case Branching:
    delete branching;
    break;
  default: break;
  }
}

Preprocessor::Preprocessor(Registers& r)
  : m_registers(r)
{
  m_state.frames.emplace_back(State::Idle);
  m_defs.push_front(Definitions{});
}

void Preprocessor::advance()
{
  if (m_input.empty())
  {
    return;
  }

  switch (m_state.frames.back().type)
  {
  case State::ReadingMacro:
    return readMacro();
  case State::ExpandingMacro:
    return expandMacro();
  case State::Branching:
    return branch();
  default:
  {
    if (peek(m_input).isCharacterToken())
    {
      parsing::write(read(m_input), m_output);
    }
    else if (peek(m_input).isControlSequence())
    {
      processControlSeq();
    }
    else
    {
      throw std::runtime_error{ "Illegal parameter token in token stream" };
    }
  }
    break;
  }
}

void Preprocessor::enter(State::FrameType s)
{
  m_state.frames.emplace_back(s);
}

void Preprocessor::leave()
{
  m_state.frames.pop_back();
}

Preprocessor::State::Frame& Preprocessor::currentFrame()
{
  return m_state.frames.back();
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

void Preprocessor::processControlSeq()
{
  Token cs = peek(m_input);

  if (cs.controlSequence() == "def")
  {
    enter(State::ReadingMacro);
    parsing::discard(m_input);
  }
  else if (cs.controlSequence() == "ifbr")
  {
    enter(State::Branching);
    currentFrame().branching->success = m_registers.br;
    parsing::discard(m_input);
  }
  else
  {
    const Macro* m = find(cs.controlSequence());

    if (m == nullptr)
    {
      parsing::write(parsing::read(m_input), m_output);
    }
    else
    {
      if (m->parameterText().empty())
      {
        parsing::discard(m_input);
        m->expand({}, m_input, m_input.begin());
      }
      else
      {
        enter(State::ExpandingMacro);
        currentFrame().macro_expansion->def = m;
        parsing::discard(m_input);
        updateExpandMacroState();
      }
    }
  }
}

void Preprocessor::readMacro()
{
  Token tok = parsing::read(m_input);
  State::Frame& frame = currentFrame();
  auto& macro_definition = *(frame.macro_definition);

  switch (currentFrame().subtype)
  {
  case State::RM_ReadingMacroName:
  {
    if (!tok.isControlSequence())
      throw std::runtime_error{ "Expected control sequence name after \\def" };

    macro_definition.csname = tok.controlSequence();
    frame.subtype = State::RM_ReadingMacroParameterText;
  }
  break;
  case State::RM_ReadingMacroParameterText:
  {
    if (tok.isCharacterToken() && tok.characterToken().value == '{')
    {
      frame.subtype = State::RM_ReadingMacroReplacementText;
      return;
    }

    if (tok.isParameterToken())
    {
      if (tok.parameterNumber() != macro_definition.parameter_index)
        throw std::runtime_error{ "Unexpected parameter number" };

      macro_definition.parameter_index += 1;
    }
    
    macro_definition.parameter_text.push_back(tok);
  }
  break;
  case State::RM_ReadingMacroReplacementText:
  {
    if (tok.isCharacterToken())
    {
      if (tok.characterToken().value == '{')
      {
        macro_definition.brace_nesting += 1;
        macro_definition.replacement_text.push_back(tok);
      }
      else if (tok.characterToken().value == '}')
      {
        if (macro_definition.brace_nesting == 0)
        {
          Macro mdef{ std::move(macro_definition.csname), std::move(macro_definition.parameter_text), std::move(macro_definition.replacement_text) };
          m_defs.front().macros[mdef.controlSequence()] = std::move(mdef);
          leave();
        }
        else
        {
          macro_definition.brace_nesting -= 1;
          macro_definition.replacement_text.push_back(tok);
        }
      }
      else
      {
        macro_definition.replacement_text.push_back(tok);
      }
    }
    else
    {
      macro_definition.replacement_text.push_back(tok);
    }
  }
  break;
  default:
    assert(false);
    break;
  }
}

void Preprocessor::updateExpandMacroState()
{
  State::Frame& frame = currentFrame();
  auto& macro_expansion = *(frame.macro_expansion);

  const std::vector<Token>& fullpat = macro_expansion.def->parameterText();

  if (macro_expansion.pattern_index == fullpat.size())
    return;

  if (fullpat.at(macro_expansion.pattern_index).isParameterToken())
  {
    macro_expansion.current_arg_brace_nesting = 0;

    if (macro_expansion.pattern_index == fullpat.size() - 1 || fullpat.at(macro_expansion.pattern_index + 1).isParameterToken())
    {
      frame.subtype = State::EXPM_ReadingUndelimitedMacroArgument;
    }
    else
    {
      frame.subtype = State::EXPM_ReadingDelimitedMacroArgument;
    }
  }
  else
  {
    frame.subtype = State::EXPM_MatchingMacroParameterText;
  }
}

void Preprocessor::expandMacro()
{
  Token tok = parsing::read(m_input);
  State::Frame& frame = currentFrame();
  auto& macro_expansion = *(frame.macro_expansion);

  auto advance_pattern = [this, &macro_expansion]() {

    macro_expansion.pattern_index++;
    updateExpandMacroState();

  };

  switch (frame.subtype)
  {
  case State::EXPM_MatchingMacroParameterText:
  {
    const Token& pat = macro_expansion.def->parameterText().at(macro_expansion.pattern_index);

    if (tok != pat)
      throw std::runtime_error{ "Use of macro does not match its definition" };

    advance_pattern();
  }
    break;
  case State::EXPM_ReadingDelimitedMacroArgument:
  {
    const Token& delimiter_pat = macro_expansion.def->parameterText().at(macro_expansion.pattern_index + 1);

    if (tok == delimiter_pat && macro_expansion.current_arg_brace_nesting == 0)
    {
      macro_expansion.pattern_index++;
      macro_expansion.current_arg_index++;
      frame.subtype = State::EXPM_MatchingMacroParameterText;
      advance_pattern();
    }
    else
    {
      macro_expansion.arguments[macro_expansion.current_arg_index].push_back(tok);

      if (tok.isCharacterToken())
      {
        if (tok.characterToken().value == '{')
        {
          macro_expansion.current_arg_brace_nesting++;
        }
        else if (tok.characterToken().value == '}')
        {
          if (macro_expansion.current_arg_brace_nesting == 0)
            throw std::runtime_error{ "Invalid nesting of '{' and '}' in delimited macro argument" };

          macro_expansion.current_arg_brace_nesting--;
        }
      }
    }
  }
    break;
  case State::EXPM_ReadingUndelimitedMacroArgument:
  {
    std::vector<Token>& current_arg_content = macro_expansion.arguments[macro_expansion.current_arg_index];

    assert(current_arg_content.empty());

    if (tok.isControlSequence() || (tok != CharCategory::GroupBegin && tok != CharCategory::Space))
    {
      current_arg_content.push_back(tok);
      macro_expansion.current_arg_index++;
      advance_pattern();
    }
    else
    {
      if (tok == CharCategory::GroupBegin)
      {
        frame.subtype = State::EXPM_ReadingBracedDelimitedMacroArgument;
        assert(macro_expansion.current_arg_brace_nesting == 0);
      }
    }
  }
    break;
  case State::EXPM_ReadingBracedDelimitedMacroArgument:
  {
    std::vector<Token>& current_arg_content = macro_expansion.arguments[macro_expansion.current_arg_index];

    if (tok == CharCategory::GroupBegin)
    {
      macro_expansion.current_arg_brace_nesting++;
      current_arg_content.push_back(tok);
    }
    else if (tok == CharCategory::GroupEnd)
    {
      if (macro_expansion.current_arg_brace_nesting == 0)
      {
        macro_expansion.current_arg_index++;
        advance_pattern();
      }
      else
      {
        macro_expansion.current_arg_brace_nesting--;
        current_arg_content.push_back(tok);
      }
    }
    else
    {
      current_arg_content.push_back(tok);
    }
  }
  break;
  default:
    assert(false);
    break;
  }

  if (macro_expansion.pattern_index == macro_expansion.def->parameterText().size())
  {
    // Done!
    macro_expansion.def->expand(macro_expansion.arguments, m_input, m_input.begin());
    leave();
  }
}

inline static bool is_if(const Token& tok)
{
  return tok.isControlSequence() && tok.controlSequence().length() >= 2
    && tok.controlSequence().at(0) == 'i' && tok.controlSequence().at(1) == 'f';
}

inline static bool is_else(const Token& tok)
{
  return tok.isControlSequence() && tok.controlSequence() == "else";
}

inline static bool is_fi(const Token& tok)
{
  return tok.isControlSequence() && tok.controlSequence() == "fi";
}

void Preprocessor::branch()
{
  Token tok = parsing::read(m_input);
  State::Frame& frame = currentFrame();
  auto& branching = *(frame.branching);

  if (is_if(tok))
  {
    branching.if_nesting += 1;
  }
  else if (is_else(tok))
  {
    if (branching.if_nesting == 0)
    {
      branching.inside_if = false;
      return;
    }
  }
  else if (is_fi(tok))
  {
    if (branching.if_nesting == 0)
    {
      m_input.insert(m_input.begin(), branching.successful_branch.begin(), branching.successful_branch.end());
      leave();
      return;
    }
    else
    {
      branching.if_nesting -= 1;
    }
  }

  if (branching.success == branching.inside_if)
  {
    branching.successful_branch.push_back(std::move(tok));
  }
}

} // namespace parsing

} // namespace tex

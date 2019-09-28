// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_PARSING_TYPESETTINGMACHINE_H
#define LIBTYPESET_PARSING_TYPESETTINGMACHINE_H

#include "tex/parsing/machine.h"

#include "tex/typeset.h"

namespace tex
{

namespace parsing
{

class LIBTYPESET_API TypesettingMachine : public Machine
{
public:
  TypesettingMachine(std::shared_ptr<TypesetEngine> te, const Options & opts);
  ~TypesettingMachine() = default;

  const std::shared_ptr<TypesetEngine>& typesetEngine() const;
  const Options& options() const;

  void beginGroup();
  void endGroup();

  bool typesetIndividualCharacters() const;

private:
  bool m_typeset_chars = false;
  std::shared_ptr<TypesetEngine> m_typeset_engine;
  std::stack<Options> m_opts;
};

class LIBTYPESET_API TextTypesetter
{
public:
  explicit TextTypesetter(std::shared_ptr<TypesetEngine> te, Font f = Font::Default, FontSize s = FontSize::Normal);
  ~TextTypesetter() = default;

  Font font() const;
  void setFont(Font f);
  FontSize fontSize() const;
  void setFontSize(FontSize fs);

  std::shared_ptr<Box> print(const std::string& text) const;

  std::string& buffer();
  const std::string& buffer() const;
  bool ready() const;
  
  std::shared_ptr<Box> print();

private:
  std::shared_ptr<TypesetEngine> m_typeset_engine;
  Font m_font;
  FontSize m_size;
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

} // namespace parsing

} // namespace tex

#endif // LIBTYPESET_PARSING_TYPESETTINGMACHINE_H

// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/parsing/typesetting-machine.h"

#include "tex/unicode.h"
#include "tex/parsing/verticalmode.h"

#include <cassert>

namespace tex
{

namespace parsing
{

TypesettingMachine::TypesettingMachine(std::shared_ptr<TypesetEngine> te, const Options& opts)
  : m_memory{},
  m_inputstream{},
  m_preprocessor{ m_registers },
  m_typeset_engine(te)
{
  m_opts.push(opts);

  enter<VerticalMode>();
}

void TypesettingMachine::beginGroup()
{
  m_preprocessor.beginGroup();
  m_lexercatcodes.push(m_lexer.catcodes());
  Options opts = m_opts.top();
  m_opts.push(opts);
}

void TypesettingMachine::endGroup()
{
  m_opts.pop();
  m_lexer.catcodes() = m_lexercatcodes.top();
  m_lexercatcodes.pop();
  m_preprocessor.endGroup();
}

TextTypesetter::TextTypesetter(std::shared_ptr<TypesetEngine> te, Font f, FontSize s)
  : m_typeset_engine(te),
  m_font(f),
  m_size(s)
{

}

Font TextTypesetter::font() const
{
  return m_font;
}

void TextTypesetter::setFont(Font f)
{
  m_font = f;
}

FontSize TextTypesetter::fontSize() const
{
  return m_size;
}

void TextTypesetter::setFontSize(FontSize fs)
{
  m_size = fs;
}

std::shared_ptr<Box> TextTypesetter::print(const std::string& text) const
{
  return m_typeset_engine->typeset(text, font(), fontSize());
}

std::string& TextTypesetter::buffer()
{
  return m_buffer;
}

const std::string& TextTypesetter::buffer() const
{
  return m_buffer;
}

bool TextTypesetter::ready() const
{
  return !buffer().empty() && is_utf8_string(buffer());
}

std::shared_ptr<Box> TextTypesetter::print()
{
  assert(ready());
  auto box = m_typeset_engine->typeset(buffer(), m_font, m_size);
  m_buffer.clear();
  return box;
}

} // namespace parsing

} // namespace tex

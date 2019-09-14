// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_TYPESETTING_INSTRUCTIONS_H
#define LIBTYPESET_TYPESETTING_INSTRUCTIONS_H

#include "tex/defs.h"

#include "tex/symbol.h"
#include "tex/font.h"

#include "tex/parsing/typesetting-visitor.h"

namespace tex
{

namespace typesetting
{

class LIBTYPESET_API TypesettingInstruction
{
public:
  virtual ~TypesettingInstruction() = default;

  virtual void accept(TypesettingVisitor& v) = 0;
};

template<typename T, typename V = TypesettingVisitor>
class GenericTypesettingInstruction : public TypesettingInstruction
{
public:

  void accept(TypesettingVisitor& v) override
  {
    return static_cast<V&>(v).visit(static_cast<T&>(*this));
  }
};

class LIBTYPESET_API TypesetText : public GenericTypesettingInstruction<TypesetText>
{
  std::shared_ptr<tex::TextSymbol> m_text;

public:
  explicit TypesetText(const std::shared_ptr<tex::TextSymbol>& text) : m_text{ text } {}
  ~TypesetText() = default;

  const std::shared_ptr<tex::TextSymbol>& text() const { return m_text; }
};

class LIBTYPESET_API InsertInterwordGlue : public GenericTypesettingInstruction<InsertInterwordGlue>
{
public:
  InsertInterwordGlue() = default;
  ~InsertInterwordGlue() = default;
};

class LIBTYPESET_API EndParagraph : public GenericTypesettingInstruction<EndParagraph>
{
public:
  EndParagraph() = default;
  ~EndParagraph() = default;
};

class LIBTYPESET_API EnterGroup : public GenericTypesettingInstruction<EnterGroup>
{
public:
  EnterGroup() = default;
  ~EnterGroup() = default;
};

class LIBTYPESET_API LeaveGroup : public GenericTypesettingInstruction<LeaveGroup>
{
public:
  LeaveGroup() = default;
  ~LeaveGroup() = default;
};

class LIBTYPESET_API ChangeFont : public GenericTypesettingInstruction<ChangeFont>
{
  tex::Font m_font;

public:
  explicit ChangeFont(const Font& f) : m_font{ f } {}
  ~ChangeFont() = default;

  Font font() const { return m_font; }
};

} // namespace typesetting

} // namespace tex

#endif // LIBTYPESET_TYPESETTING_INSTRUCTIONS_H

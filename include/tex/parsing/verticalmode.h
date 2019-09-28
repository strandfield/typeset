// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_PARSING_VERTICALMODE_H
#define LIBTYPESET_PARSING_VERTICALMODE_H

#include "tex/parsing/mode.h"

#include "tex/parsing/commands.h"

#include "tex/vbox.h"
#include "tex/typeset.h"

namespace tex
{

namespace parsing
{

class TypesettingMachine;

class LIBTYPESET_API VerticalMode : public Mode
{
public:
  VerticalMode(TypesettingMachine& m);
  ~VerticalMode() = default;

  typedef RetCode(*Callback)(VerticalMode&);

  TypesettingMachine& machine() const;
  std::map<std::string, Callback>& commands();

  int pageWidth() const;
  void setPageWidth(int pw);

  RetCode advance() override;

  List& vlist();

  /* Callbacks */
  static RetCode main_callback(VerticalMode&);

private:
  std::vector<Callback> m_callbacks;
  std::map<std::string, Callback> m_commands;
  List m_vlist;
  int m_pagewidth = 600;
};

} // namespace parsing

} // namespace tex

namespace tex
{

namespace parsing
{

inline std::map<std::string, VerticalMode::Callback>& VerticalMode::commands()
{
  return m_commands;
}

inline int VerticalMode::pageWidth() const
{
  return m_pagewidth;
}

inline void VerticalMode::setPageWidth(int pw)
{
  m_pagewidth = pw;
}

} // namespace parsing

} // namespace tex

#endif // LIBTYPESET_PARSING_VERTICALMODE_H

// Copyright (C) 2019-2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef TYPESET_PAGEEDITOR_HORIZONTALMODE_H
#define TYPESET_PAGEEDITOR_HORIZONTALMODE_H

#include "mode.h"

#include "typesetting-machine.h"

#include "common/charbuffer.h"

#include "tex/fontmetrics.h"
#include "tex/hbox.h"
#include "tex/typeset.h"

#include "tex/parsing/kernparser.h"

class HorizontalMode : public Mode
{
public:
  HorizontalMode(TypesettingMachine& m);
  ~HorizontalMode() = default;

  enum class State
  {
    Main,
    MathShift,
    Kern,
  };

  tex::FontMetrics metrics() const;

  enum class CS
  {
    PAR,
    KERN,
    HBOX,
  };

  static const std::map<std::string, CS>& csmap();
  static CS controlSequence(const std::string& name);

  Kind kind() const override;
  void write(tex::parsing::Token& t) override;
  void write(std::shared_ptr<tex::ListBox> box);
  void finish() override;

  tex::Font currentFont() const;

  tex::List& hlist();

  void writeOutput();

protected:
  void write_main(tex::parsing::Token&);
  void write_mathshift(tex::parsing::Token&);
  void write_kern(tex::parsing::Token&);

  void beginGroup();
  void endGroup();

  void par_callback();
  void kern_callback();
  void hbox_callback();

private:
  bool m_is_restricted = false;
  State m_state = State::Main;
  CharBuffer m_buffer;
  std::unique_ptr<tex::parsing::KernParser> m_kern_parser;
  tex::List m_hlist;
};

#endif // TYPESET_PAGEEDITOR_HORIZONTALMODE_H

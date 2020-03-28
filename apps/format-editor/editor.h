// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_FORMARTEDITOR_EDITOR_H
#define LIBTYPESET_FORMARTEDITOR_EDITOR_H

#include <QPlainTextEdit>
#include <QSyntaxHighlighter>

#include <memory>
#include <string>
#include <utility>

struct TextFragment
{
  int start = -1;
  int length = 0;
};

enum class Label
{
  Invalid = 0,
  Def,
  Cs,
  Tok,
  Param,
  ReplDelim,
  Save,
};

struct TextLabelization
{
  TextFragment fragment;
  Label label = Label::Invalid;
};

class TextBlockData : public QTextBlockUserData
{
public:
  std::vector<TextLabelization> labels;
};

class SyntaxHighlighter : public QSyntaxHighlighter
{
  Q_OBJECT
public:
  SyntaxHighlighter(QTextDocument* parent = 0);

protected:
  void highlightBlock(const QString& text) override;

private:
  const QTextCharFormat& formatForLabel(Label l) const;

private:
  QTextCharFormat m_default_format;
  QTextCharFormat m_def_format;
  QTextCharFormat m_seq_format;
  QTextCharFormat m_param_format;
  QTextCharFormat m_tok_format;
  QTextCharFormat m_repl_format;
  QTextCharFormat m_save_format;
};

class Editor : public QPlainTextEdit
{
  Q_OBJECT
public:
  explicit Editor(QWidget* parent = nullptr);

  std::string computeFormat() const;
  void loadFormat(const std::string& fmt);

protected Q_SLOTS:
  void autoLabelizeSelection();

protected:
  void contextMenuEvent(QContextMenuEvent* ev) override;

private:
  SyntaxHighlighter* m_highlighter;
};

#endif // LIBTYPESET_FORMARTEDITOR_EDITOR_H

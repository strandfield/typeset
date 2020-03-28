// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "editor.h"

#include <QFont>

#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>

#include <QTextCursor>

inline static bool is_tex_escape(QChar c)
{
  return c.toLatin1() == '\\';
}

inline static bool is_tex_letter(QChar c)
{
  char ch = c.toLatin1();
  return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch == '@');
}

inline static void replace(std::string& self, const std::string& pattern, const std::string& repl)
{
  size_t pos = self.find(pattern);

  while (pos != std::string::npos)
  {
    self.replace(self.begin() + pos, self.begin() + pos + pattern.size(), repl);
    pos = self.find(pattern, pos + repl.size());
  }
}

inline static std::string format_export(std::string str)
{
  replace(str, "\\", "\\\\");
  replace(str, "\"", "\\\"");

  return str;
}

inline static std::string encode_length(int l)
{
  return std::string(1, char(l + '0'));
}

inline static std::string encode(QTextBlock& b, const std::vector<TextLabelization>& labels)
{
  std::string result;

  size_t off = 0;
  size_t i = 0;

  for (const TextLabelization& labelization : labels)
  {

    if (off < labelization.fragment.start)
      result += '>' + encode_length(labelization.fragment.start - off);

    off = labelization.fragment.start;

    switch (labelization.label)
    {
    case Label::Def:
      result += "d" + encode_length(labelization.fragment.length - 4 - 1);
      off += labelization.fragment.length;
      break;
    case Label::Cs:
      result += "c" + encode_length(labelization.fragment.length - 1);
      off += labelization.fragment.length;
      break;
    case Label::Tok:
      result += 't';
      off += 1;
      break;
    case Label::Param:
      result += 'p';
      off += 2;
      break;
    case Label::ReplDelim:
      result += 'r';
      off++;
      break;
    case Label::Save:
      result += 's';
      off++;
      break;
    }

  }

  if (off < b.length() - 1)
  {
    result += '>' + encode_length(b.length() - 1 - off);
  }

  return result;
}


inline static std::string encode(QTextBlock& b, TextBlockData* data)
{
  if (b.length() == 1)
    return std::string();

  if (!data)
    return ">" + encode_length(b.length() - 1);
  else
    return encode(b, data->labels);
}

struct Token
{
  QTextBlock block;
  int start;
  int length;
  char type; // 't', 'c', 'p'

  QString text() const
  {
    return block.text().mid(start, length);
  }
};

class Labelizer
{
private:
  QTextBlock m_begin;
  QTextBlock m_end;
  QTextBlock m_cur_block;
  QString m_cur_text;
  int m_cur_pos = 0;

public:
  void labelize(QTextBlock begin, QTextBlock end)
  {
    QTextBlock b = begin;
    
    while (b != end)
    {
      clearLabels(b);
      b = b.next();
    }

    m_begin = begin;
    m_end = end;

    m_cur_block = m_begin;
    m_cur_text = m_cur_block.text();
    m_cur_pos = 0;

    Token def_tok = readToken();

    if (def_tok.text() != "\\def")
      return;

    Token macro_tok = readToken();

    if (macro_tok.type != 'c')
      return;

    def_tok.length += macro_tok.length;
    labelize(def_tok, Label::Def);

    Token tok = readToken();

    while (tok.text() != "{")
    {
      if (tok.type == 'c')
        labelize(tok, Label::Cs);
      else if (tok.type == 'p')
        labelize(tok, Label::Param);
      else if (tok.type == 't')
        labelize(tok, Label::Tok);

      tok = readToken();
    }

    labelize(tok, Label::ReplDelim);

    int brace_depth = 1;

    while (!atEnd())
    {
      tok = readToken();

      if (tok.type == 'c')
      {
        labelize(tok, Label::Cs);
      }
      else if (tok.type == 'p')
      {
        labelize(tok, Label::Param);
      }
      else if (tok.type == 't')
      {
        QString text = tok.text();

        if (text == "}")
        {
          brace_depth -= 1;

          if (brace_depth == 0)
          {
            labelize(tok, Label::Save);
            return;
          }

          labelize(tok, Label::Tok);
        }
        else
        {
          if (text == "{")
            brace_depth += 1;

          labelize(tok, Label::Tok);
        }
      }
    }
  }

  static void labelize(QTextBlock& block, int start, int len, Label label)
  {
    if (!block.userData())
      block.setUserData(new TextBlockData());
    
    TextLabelization labelization;
    labelization.fragment.start = start;
    labelization.fragment.length = len;
    labelization.label = label;

    static_cast<TextBlockData*>(block.userData())->labels.push_back(labelization);
  }

  void labelize(Token tok, Label label)
  {
    labelize(tok.block, tok.start, tok.length, label);
  }

  void clearLabels(QTextBlock& block)
  {
    if (!block.userData())
      return;

    static_cast<TextBlockData*>(block.userData())->labels.clear();
  }

  void readSpaces()
  {
    while (m_cur_block != m_end)
    {
      while (m_cur_pos < m_cur_text.length())
      {
        if (m_cur_text.at(m_cur_pos) != ' ')
          return;
        else
          m_cur_pos++;
      }

      m_cur_block = m_cur_block.next();
      m_cur_text = m_cur_block.text();
      m_cur_pos = 0;
    }
  }

  bool atEnd() const
  {
    return m_cur_block == m_end;
  }

  Token readToken()
  {
    if (atEnd())
      throw std::runtime_error{ "At end..." };

    Token ret;
    ret.block = m_cur_block;
    ret.start = m_cur_pos;

    if (m_cur_text.at(m_cur_pos) == '\\')
    {
      ++m_cur_pos;

      while (m_cur_pos < m_cur_text.length() && is_tex_letter(m_cur_text.at(m_cur_pos)))
        ++m_cur_pos;

      ret.length = m_cur_pos - ret.start;
      ret.type = 'c';
    }
    else if (m_cur_text.at(m_cur_pos) == '#')
    {
      m_cur_pos += 2;
      ret.length = 2;
      ret.type = 'p';
    }
    else
    {
      m_cur_pos += 1;
      ret.length = 1;
      ret.type = 't';
    }

    readSpaces();
    return ret;
  }

};

class FormatLoader
{
private:
  QTextBlock& block;
  size_t off = 0;
  const char* bytecode = nullptr;

private:
  FormatLoader(QTextBlock& b)
    : block(b)
  {

  }

  void labelize(const char* bytecode_)
  {
    bytecode = bytecode_;

    while (*bytecode != '\0')
    {
      switch (*(bytecode++))
      {
      case '>':
        skip();
        break;
      case 'd':
        def();
        break;
      case 'r':
        repl();
        break;
      case 'p':
        param();
        break;
      case 't':
        tok();
        break;
      case 'c':
        cs();
        break;
      case 's':
        save();
        break;
      }
    }
  }


  void skip()
  {
    off += static_cast<size_t>(*(bytecode++) - '0');
  }

  void def()
  {
    size_t size = static_cast<size_t>(*(bytecode++) - '0');
    size += 4 + 1;

    Labelizer::labelize(block, off, size, Label::Def);
    off += size;
  }

  void repl()
  {
    Labelizer::labelize(block, off, 1, Label::ReplDelim);
    off += 1;
  }

  void param()
  {
    Labelizer::labelize(block, off, 2, Label::Param);
    off += 2;
  }

  void tok()
  {
    Labelizer::labelize(block, off, 1, Label::Tok);
    off += 1;
  }

  void cs()
  {
    size_t size = static_cast<size_t>(*(bytecode++) - '0');
    size += 1;
   
    Labelizer::labelize(block, off, size, Label::Cs);
    off += size;
  }

  void save()
  {
    Labelizer::labelize(block, off, 1, Label::Save);
    off += 1;
  }

public:
  static void labelize(QTextBlock& block, const std::string& bytecode)
  {
    if (bytecode.empty())
      return;

    FormatLoader loader{ block };
    loader.labelize(bytecode.c_str());
  }
};

SyntaxHighlighter::SyntaxHighlighter(QTextDocument* parent)
  : QSyntaxHighlighter(parent)
{
  m_def_format.setForeground(Qt::darkRed);
  m_seq_format.setForeground(Qt::blue);
  m_param_format.setForeground(Qt::yellow);
  m_tok_format.setForeground(Qt::magenta);
  m_repl_format.setForeground(Qt::darkRed);
  m_save_format.setForeground(Qt::darkRed);
}

void SyntaxHighlighter::highlightBlock(const QString& text)
{
  TextBlockData* data = dynamic_cast<TextBlockData*>(currentBlock().userData());

  if (!data)
    return;

  for (const TextLabelization& l : data->labels)
  {
    setFormat(l.fragment.start, l.fragment.length, formatForLabel(l.label));
  }
}

const QTextCharFormat& SyntaxHighlighter::formatForLabel(Label l) const
{
  switch (l)
  {
  case Label::Cs:
    return m_seq_format;
  case Label::Def:
    return m_def_format;
  case Label::Param:
    return m_param_format;
  case Label::Tok:
    return m_tok_format;
  case Label::ReplDelim:
    return m_repl_format;
  case Label::Save:
    return m_save_format;
  default:
    return m_default_format;
  }
}

Editor::Editor(QWidget* parent)
  : QPlainTextEdit(parent)
{
  m_highlighter = new SyntaxHighlighter(document());

  QFont font = document()->defaultFont();
  font.setFamily("Courier New");
  document()->setDefaultFont(font);
}

std::string Editor::computeFormat() const
{
  std::string result;

  result += "const char* fmt_src =\n";

  for (QTextBlock b = document()->begin(); b != document()->end(); b = b.next())
  {
    std::string content = b.text().toStdString();
    content = format_export(content);
    result += "  \"" + content + "\"";
    
    if (b.blockNumber() % 5 == 0)
      result += " // " + std::to_string(b.blockNumber());
    result += "\n";
  }

  result += ";\n\n";

  result += "const char* fmt_bytecode =\n";
  result += "  \"v0\"\n";

  for (QTextBlock b = document()->begin(); b != document()->end(); b = b.next())
  {
    std::string content = encode(b, static_cast<TextBlockData*>(b.userData()));
    result += "  \"" + content + "\"";

    if (b.blockNumber() % 5 == 0)
      result += " // " + std::to_string(b.blockNumber());
    result += "\n";
  }

  result += "  \"e\";\n\n";

  return result;
}

void Editor::loadFormat(const std::string& fmt)
{
  document()->clear();
  QTextCursor cursor{ document() };

  size_t off = 0;

  // Parse format content
  {
    size_t pos = fmt.find("const char* ");

    if (pos == std::string::npos)
      return;
    
    pos = fmt.find("\n", pos) + 1;

    while (fmt[pos] != ';')
    {
      const std::string line = fmt.substr(pos, fmt.find('\n', pos) - pos);

      {
        size_t line_begin = line.find('"') + 1;
        size_t line_end = line.rfind('"');

        std::string line_content = line.substr(line_begin, line_end - line_begin);
        replace(line_content, "\\\\", "\\");
        cursor.insertText(QString::fromStdString(line_content));
        cursor.insertBlock();
      }

      pos += line.length() + 1;
    }

    // We have one block too many
    cursor.deletePreviousChar();

    off = pos;
  }

  // Parse labels
  {
    size_t pos = fmt.find("const char* ", off);

    if (pos == std::string::npos)
      return;

    pos = fmt.find("\n", pos) + 1;
    pos = fmt.find("\n", pos) + 1;

    QTextBlock block = document()->begin();

    while (block != document()->end())
    {
      const std::string line = fmt.substr(pos, fmt.find('\n', pos) - pos);

      {
        size_t line_begin = line.find('"') + 1;
        size_t line_end = line.rfind('"');

        std::string line_content = line.substr(line_begin, line_end - line_begin);
        FormatLoader::labelize(block, line_content);
      }

      pos += line.length() + 1;
      block = block.next();
    }
  }

  m_highlighter->rehighlight();
}

void Editor::autoLabelizeSelection()
{
  QTextCursor cursor = textCursor();

  int begin = cursor.position();
  int end = cursor.anchor();

  if (begin > end)
    std::swap(begin, end);

  QTextBlock begin_block = document()->findBlock(begin);
  QTextBlock end_block = document()->findBlock(end).next();

  Labelizer labelizer;
  labelizer.labelize(begin_block, end_block);

  m_highlighter->rehighlight();
}

void Editor::contextMenuEvent(QContextMenuEvent* ev)
{
  QMenu* menu = new QMenu;
  QAction* labelize = menu->addAction("Auto labelize");
 
  connect(labelize, &QAction::triggered, this, &Editor::autoLabelizeSelection);

  menu->exec(ev->globalPos());
  delete menu;
}

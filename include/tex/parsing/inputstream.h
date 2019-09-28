// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_PARSING_INPUTSTREAM_H
#define LIBTYPESET_PARSING_INPUTSTREAM_H

#include "tex/token.h"

#include <list>

namespace tex
{

namespace parsing
{

struct Memory;

class LIBTYPESET_API InputStream
{
public:
  InputStream();
  InputStream(const InputStream&) = default;
  InputStream(InputStream&&) noexcept = default;

  struct Document
  {
    std::string filepath;
    std::string content;
    int offset = 0;
    int line = 0;
    int col = 0;

    static Document fromString(std::string&& str);
    static Document fromLocalFile(const std::string& path);
  };
  
  bool atEnd() const;

  char read();
  char peek() const;

  const Document& currentDocument() const;

  void push_front(Document&& doc);
  void push_back(Document&& doc);

private:
  std::list<Document> m_documents;
};

} // namespace parsing

} // namespace tex

namespace tex
{

namespace parsing
{

inline InputStream::InputStream()
{

}

inline bool InputStream::atEnd() const
{
  return m_documents.empty();
}

inline char InputStream::read()
{
  Document& doc = m_documents.front();
  char c = doc.content[doc.offset++];
  doc.col += 1;

  if (c == '\n')
  {
    doc.col = 0;
    doc.line += 1;
  }

  if (doc.offset == doc.content.size())
  {
    m_documents.pop_front();
  }

  return c;
}

inline char InputStream::peek() const
{
  const Document& doc = currentDocument();
  return doc.content[doc.offset];
}

inline const InputStream::Document& InputStream::currentDocument() const
{
  return m_documents.front();
}

inline void InputStream::push_front(Document&& doc)
{
  m_documents.push_front(std::move(doc));
}

inline void InputStream::push_back(Document&& doc)
{
  m_documents.push_back(std::move(doc));
}

} // namespace parsing

} // namespace tex


#endif // LIBTYPESET_PARSING_INPUTSTREAM_H

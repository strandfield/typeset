// Copyright (C) 2018 Vincent Chambrin
// This file is part of the liblayout project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "textprocessor.h"

#include <QFontMetricsF>

TextProcessor::TextProcessor(const QFont & f)
  : font_(f)
{
  QFontMetricsF metrics{ f };
  xHeight = metrics.xHeight();
  em = metrics.width(QChar('M'));
  const float space = metrics.width(QChar(' '));

  spaceglue = std::make_shared<tex::Glue>(space, 0.25f * space, 1.f * space);
  commaglue = std::make_shared<tex::Glue>(space, 0.25f * space, 1.5f * space);
  periodglue = std::make_shared<tex::Glue>(space, 0.25f * space, 2.f * space);
}

QString TextProcessor::readToken(const QString & text, int & pos)
{
  if (pos == text.size())
    return QString();

  while (pos != text.size() && text.at(pos).isSpace())
    ++pos;

  if (pos == text.size())
    return QString();

  if (text.at(pos).isPunct())
  {
    const int start = pos;
    while (pos < text.size() && text.at(pos).isPunct())
      ++pos;

    return text.mid(start, pos - start);
  }
  else
  {
    const int start = pos;
    while (pos < text.size() && !text.at(pos).isPunct() && !text.at(pos).isSpace())
      ++pos;

    return text.mid(start, pos - start);
  }
}

tex::List TextProcessor::process(const QString & text)
{
  tex::List result;

  int pos = 0;
  QString w = readToken(text, pos);
  bool insert_space = false;

  while (!w.isNull())
  {
    if (w.size() == 1 && w.front().isPunct())
    {
      insert_space = false;
      result.push_back(stringbox(w, font_));
      if (w.front() == ',')
        result.push_back(commaglue);
      else if (w.front() == '.')
        result.push_back(periodglue);
      else if(w.front() != '-' && w.front() != '\'')
        result.push_back(spaceglue);
    }
    else if (w == "...")
    {
      insert_space = false;
      result.push_back(stringbox(w, font_));
      result.push_back(periodglue);
    }
    else
    {
      if (insert_space)
        result.push_back(spaceglue);

      result.push_back(stringbox(w, font_));
      insert_space = true;
    }

    w = readToken(text, pos);
  }

  return result;
}

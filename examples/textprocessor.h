// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_EXAMPLES_TEXTPROCESSOR_H
#define LIBTYPESET_EXAMPLES_TEXTPROCESSOR_H

#include "stringbox.h"

#include <tex/glue.h>
#include <tex/listbox.h>

#include <QFont>
#include <QString>

class TextProcessor
{
public:
  QFont font_;
  float xHeight;
  float em;
  std::shared_ptr<tex::Glue> spaceglue;
  std::shared_ptr<tex::Glue> commaglue;
  std::shared_ptr<tex::Glue> periodglue;

  TextProcessor(const QFont & f);

  tex::List process(const QString & text);

  static QString readToken(const QString & text, int & pos);
};

#endif // LIBTYPESET_EXAMPLES_TEXTPROCESSOR_H

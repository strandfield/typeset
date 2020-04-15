// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_EQEDITOR_SUGGESTIONBAR_H
#define LIBTYPESET_EQEDITOR_SUGGESTIONBAR_H

#include <QWidget>

#include <string>

class SuggestionBar : public QWidget
{
  Q_OBJECT
public:
  explicit SuggestionBar(QWidget* parent = nullptr);

  void addSuggestion(const QString& display_text, const std::string& content);
  void addSuggestion(const std::string& content);

  void clear();

Q_SIGNALS:
  void suggestionSelected(const std::string& content);

};

#endif // LIBTYPESET_EQEDITOR_SUGGESTIONBAR_H

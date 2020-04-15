// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "suggestionbar.h"

#include <QHBoxLayout>
#include <QPushButton>

SuggestionBar::SuggestionBar(QWidget* parent)
  : QWidget(parent)
{
  auto* layout = new QHBoxLayout;
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addStretch(1);
  setLayout(layout);
}

void SuggestionBar::addSuggestion(const QString& display_text, const std::string& content)
{
  QHBoxLayout* l = static_cast<QHBoxLayout*>(layout());

  QPushButton* button = new QPushButton(display_text, this);
  l->insertWidget(l->count() - 1, button);

  connect(button, &QPushButton::clicked, [this, content]() {
    Q_EMIT suggestionSelected(content);
    });
}

void SuggestionBar::addSuggestion(const std::string& content)
{
  addSuggestion(QString::fromStdString(content), content);
}

void SuggestionBar::clear()
{
  QHBoxLayout* l = static_cast<QHBoxLayout*>(layout());
 
  for (int i(0); i < l->count(); ++i)
  {
    if (l->itemAt(i)->widget() != nullptr)
      l->itemAt(i)->widget()->deleteLater();
  }
}

// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_EQEDITOR_FONTTREEWIDGET_H
#define LIBTYPESET_EQEDITOR_FONTTREEWIDGET_H

#include <QTreeWidget>

#include <memory>

class TypesetEngine;

class FontTreeWidget : public QTreeWidget
{
  Q_OBJECT
public:
  explicit FontTreeWidget(std::shared_ptr<TypesetEngine> engine, QWidget* parent = nullptr);

  enum Role
  {
    FontIdRole = Qt::UserRole + 1,
    FamilyRole,
    FontDimenNameRole,
  };

  enum FontDimenName
  {
    SlantPerPt = 0,
    InterwordSpace,
    InterwordStretch,
    InterwordShrink,
    XHeight,
    Quad,
    ExtraSpace,
    Num1,
    Num2,
    Num3,
    Denom1,
    Denom2,
    Sup1,
    Sup2,
    Sup3,
    Sub1,
    Sub2,
    SupDrop,
    SubDrop,
    Delim1,
    Delim2,
    AxisHeight,
    DefaultRuleThickness,
    BigOpSpacing1,
    BigOpSpacing2,
    BigOpSpacing3,
    BigOpSpacing4,
    BigOpSpacing5,
  };

  void showOnlyUsedFontDimen(bool on);

  void sync();

protected:
  void createItems(int fam);
  QTreeWidgetItem* createItem(const QString& name, int fam, int id);
  QTreeWidgetItem* createFontDimenItem(const QString& text, FontDimenName name);

private:
  std::shared_ptr<TypesetEngine> m_engine;
  bool m_show_only_used_fontdimen = false;
};

#endif // LIBTYPESET_EQEDITOR_FONTTREEWIDGET_H

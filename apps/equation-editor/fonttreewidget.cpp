// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "fonttreewidget.h"

#include "typeset-engine.h"

template<typename T>
auto get_field(FontTreeWidget::FontDimenName name, const T& fontdimen) -> decltype(fontdimen.quad)
{
  switch (name)
  {
  case FontTreeWidget::SlantPerPt:
    return fontdimen.slant_per_pt;
  case FontTreeWidget::InterwordSpace:
    return fontdimen.interword_space;
  case FontTreeWidget::InterwordStretch:
    return fontdimen.interword_stretch;
  case FontTreeWidget::InterwordShrink:
    return fontdimen.interword_shrink;
  case FontTreeWidget::XHeight:
    return fontdimen.x_height;
  case FontTreeWidget::Quad:
    return fontdimen.quad;
  case FontTreeWidget::ExtraSpace:
    return fontdimen.extra_space;
  case FontTreeWidget::Num1:
    return fontdimen.num1;
  case FontTreeWidget::Num2:
    return fontdimen.num2;
  case FontTreeWidget::Num3:
    return fontdimen.num3;
  case FontTreeWidget::Denom1:
    return fontdimen.denom1;
  case FontTreeWidget::Denom2:
    return fontdimen.denom2;
  case FontTreeWidget::Sup1:
    return fontdimen.sup1;
  case FontTreeWidget::Sup2:
    return fontdimen.sup2;
  case FontTreeWidget::Sup3:
    return fontdimen.sup3;
  case FontTreeWidget::Sub1:
    return fontdimen.sub1;
  case FontTreeWidget::Sub2:
    return fontdimen.sub2;
  case FontTreeWidget::SupDrop:
    return fontdimen.sup_drop;
  case FontTreeWidget::SubDrop:
    return fontdimen.sub_drop;
  case FontTreeWidget::Delim1:
    return fontdimen.delim1;
  case FontTreeWidget::Delim2:
    return fontdimen.delim2;
  case FontTreeWidget::AxisHeight:
    return fontdimen.axis_height;
  case FontTreeWidget::DefaultRuleThickness:
    return fontdimen.default_rule_thickness;
  case FontTreeWidget::BigOpSpacing1:
    return fontdimen.big_op_spacing1;
  case FontTreeWidget::BigOpSpacing2:
    return fontdimen.big_op_spacing2;
  case FontTreeWidget::BigOpSpacing3:
    return fontdimen.big_op_spacing3;
  case FontTreeWidget::BigOpSpacing4:
    return fontdimen.big_op_spacing4;
  case FontTreeWidget::BigOpSpacing5:
    return fontdimen.big_op_spacing5;
  default:
    assert(false);
    return fontdimen.quad;
  }
}

FontTreeWidget::FontTreeWidget(std::shared_ptr<RecordingTypesetEngine> engine, QWidget* parent)
  : QTreeWidget(parent),
    m_engine(engine)
{
  setColumnCount(2);
  setHeaderLabels(QStringList() << "Name" << "Value");

  createItems(0);
  createItems(1);
  createItems(2);
  createItems(3);

  sync();
}

void FontTreeWidget::createItems(int fam)
{
  addTopLevelItem(createItem("textfont", fam, fam * 3));
  addTopLevelItem(createItem("scriptfont", fam, fam * 3 + 1));
  addTopLevelItem(createItem("scriptscriptfont", fam, fam * 3 + 2));
}

QTreeWidgetItem* FontTreeWidget::createItem(const QString& name, int fam, int id)
{
  const Font& font = m_engine->fonts().at(id);

  QTreeWidgetItem* result = new QTreeWidgetItem;
  result->setData(0, FontIdRole, id);
  result->setData(0, FamilyRole, fam);

  result->setText(0, name + QString::number(fam));
  result->setText(1, font.font.toString());

  if (fam == 0 || fam == 1)
  {
    result->addChild(createFontDimenItem("slant_per_pt", SlantPerPt));
    result->addChild(createFontDimenItem("interword_space", InterwordSpace));
    result->addChild(createFontDimenItem("interword_stretch", InterwordStretch));
    result->addChild(createFontDimenItem("interword_shrink", InterwordShrink));
    result->addChild(createFontDimenItem("x_height", XHeight));
    result->addChild(createFontDimenItem("quad", Quad));

    if(fam == 0)
      result->addChild(createFontDimenItem("extra_space", ExtraSpace));
  }
  else if (fam == 2)
  {
    result->addChild(createFontDimenItem("slant_per_pt", SlantPerPt));
    result->addChild(createFontDimenItem("interword_space", InterwordSpace));
    result->addChild(createFontDimenItem("interword_stretch", InterwordStretch));
    result->addChild(createFontDimenItem("interword_shrink", InterwordShrink));
    result->addChild(createFontDimenItem("x_height", XHeight));
    result->addChild(createFontDimenItem("quad", Quad));
    result->addChild(createFontDimenItem("extra_space", ExtraSpace));
    result->addChild(createFontDimenItem("num1", Num1));
    result->addChild(createFontDimenItem("num2", Num2));
    result->addChild(createFontDimenItem("num3", Num3));
    result->addChild(createFontDimenItem("denom1", Denom1));
    result->addChild(createFontDimenItem("denom2", Denom2));
    result->addChild(createFontDimenItem("sup1", Sup1));
    result->addChild(createFontDimenItem("sup2", Sup2));
    result->addChild(createFontDimenItem("sup3", Sup3));
    result->addChild(createFontDimenItem("sub1", Sub1));
    result->addChild(createFontDimenItem("sub2", Sub2));
    result->addChild(createFontDimenItem("sup_drop", SupDrop));
    result->addChild(createFontDimenItem("sub_drop", SubDrop));
    result->addChild(createFontDimenItem("delim1", Delim1));
    result->addChild(createFontDimenItem("delim2", Delim2));
    result->addChild(createFontDimenItem("axis_height", AxisHeight));
  }
  else if (fam == 3)
  {
    result->addChild(createFontDimenItem("slant_per_pt", SlantPerPt));
    result->addChild(createFontDimenItem("interword_space", InterwordSpace));
    result->addChild(createFontDimenItem("interword_stretch", InterwordStretch));
    result->addChild(createFontDimenItem("interword_shrink", InterwordShrink));
    result->addChild(createFontDimenItem("x_height", XHeight));
    result->addChild(createFontDimenItem("quad", Quad));
    result->addChild(createFontDimenItem("extra_space", ExtraSpace));
    result->addChild(createFontDimenItem("default_rule_thickness", DefaultRuleThickness));
    result->addChild(createFontDimenItem("big_op_spacing1", BigOpSpacing1));
    result->addChild(createFontDimenItem("big_op_spacing2", BigOpSpacing2));
    result->addChild(createFontDimenItem("big_op_spacing3", BigOpSpacing3));
    result->addChild(createFontDimenItem("big_op_spacing4", BigOpSpacing4));
    result->addChild(createFontDimenItem("big_op_spacing5", BigOpSpacing5));
  }


  return result;
}

QTreeWidgetItem* FontTreeWidget::createFontDimenItem(const QString& text, FontDimenName name)
{
  auto result = new QTreeWidgetItem;
  result->setData(0, FontDimenNameRole, (int)name);
  result->setText(0, text);
  return result;
}

void FontTreeWidget::showOnlyUsedFontDimen(bool on)
{
  if (m_show_only_used_fontdimen != on)
  {
    m_show_only_used_fontdimen = on;
    sync();
  }
}

void FontTreeWidget::sync()
{
  for (int i(0); i < topLevelItemCount(); ++i)
  {
    QTreeWidgetItem* toplevel_item = topLevelItem(i);
    int fontid = toplevel_item->data(0, FontIdRole).toInt();
    const auto& fontinfo = m_engine->fonts().at(fontid);
    const auto& fontusage = m_engine->fontdimenUsage().at(fontid);

    for (int j(0); j < toplevel_item->childCount(); ++j)
    {
      QTreeWidgetItem* fontdimen_item = toplevel_item->child(j);
      FontDimenName name = static_cast<FontDimenName>(fontdimen_item->data(0, FontDimenNameRole).toInt());

      bool used = get_field(name, fontusage);
      fontdimen_item->setHidden(m_show_only_used_fontdimen && !used);

      fontdimen_item->setText(1, QString::number(get_field(name, fontinfo.fontdimen)));
    }
  }
}

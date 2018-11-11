
#ifndef LIBLAYOUT_LAYOUTREADER_H
#define LIBLAYOUT_LAYOUTREADER_H

#include "tex/glue.h"
#include "tex/hbox.h"
#include "tex/kern.h"
#include "tex/rule.h"
#include "tex/vbox.h"

#include <cassert>

namespace tex
{

struct Pos
{
  float x;
  float y;
};

struct LIBLAYOUT_API LayoutReader { };

template<typename Reader>
void read_hbox(Reader && reader, const std::shared_ptr<HBox> & layout, Pos pos)
{
  reader(layout, pos);

  const float baseline = pos.y + layout->height();

  for (auto node : layout->list())
  {
    if (node->isBox())
    {
      auto box = std::static_pointer_cast<Box>(node);

      if (box->is<Rule>())
      {
        reader(std::static_pointer_cast<Rule>(box), Pos{ pos.x, baseline - box->height() });
      }
      else if (box->isDerivedFrom<ListBox>())
      {
        auto listbox = std::static_pointer_cast<ListBox>(box);
        const float shifted_baseline = baseline + listbox->shiftAmount();
        if (listbox->is<HBox>())
        {
          read_hbox(reader, std::static_pointer_cast<HBox>(listbox), Pos{ pos.x, shifted_baseline - listbox->height() });
        }
        else
        {
          assert(listbox->is<VBox>());
          read_vbox(reader, std::static_pointer_cast<VBox>(listbox), Pos{ pos.x, shifted_baseline - listbox->height() });
        }
      }
      else
      {
        reader(box, Pos{ pos.x, baseline - box->height() });
      }

      pos.x += box->width();
    }
    else if (node->is<Kern>())
    {
      pos.x += std::static_pointer_cast<Kern>(node)->space();
    }
    else if (node->is<Glue>())
    {
      auto glue = std::static_pointer_cast<Glue>(node);

      pos.x += glue->space();

      if (layout->glueRatio() < 0.f)
      {
        if (layout->glueOrder() == glue->shrinkOrder())
          pos.x += layout->glueRatio() * glue->shrink();
      }
      else
      {
        if (layout->glueOrder() == glue->stretchOrder())
          pos.x += layout->glueRatio() * glue->stretch();
      }
    }
  }
}

template<typename Reader>
void read_vbox(Reader && reader, const std::shared_ptr<VBox> & layout, Pos pos)
{
  reader(layout, pos);

  for (auto node : layout->list())
  {
    if (node->isBox())
    {
      auto box = std::static_pointer_cast<Box>(node);

      if (box->is<Rule>())
      {
        reader(std::static_pointer_cast<Rule>(box), Pos{ pos.x, pos.y });
      }
      else if (box->isDerivedFrom<ListBox>())
      {
        auto listbox = std::static_pointer_cast<ListBox>(box);
        const float shift = listbox->shiftAmount();
        if (listbox->is<HBox>())
        {
          read_hbox(reader, std::static_pointer_cast<HBox>(listbox), Pos{ pos.x + shift, pos.y });
        }
        else
        {
          assert(listbox->is<VBox>());
          read_vbox(reader, std::static_pointer_cast<VBox>(listbox), Pos{ pos.x + shift, pos.y });
        }
      }
      else
      {
        reader(box, Pos{ pos.x, pos.y });
      }

      pos.y += box->totalHeight();
    }
    else if (node->is<Kern>())
    {
      pos.y += std::static_pointer_cast<Kern>(node)->space();
    }
    else if (node->is<Glue>())
    {
      auto glue = std::static_pointer_cast<Glue>(node);

      pos.y += glue->space();

      if (layout->glueRatio() < 0.f)
      {
        if (layout->glueOrder() == glue->shrinkOrder())
          pos.y += layout->glueRatio() * glue->shrink();
      }
      else
      {
        if (layout->glueOrder() == glue->stretchOrder())
          pos.y += layout->glueRatio() * glue->stretch();
      }
    }
  }
}

template<typename Reader, typename = std::enable_if_t<std::is_base_of_v<LayoutReader, Reader>>> 
void read(Reader && reader, const std::shared_ptr<Box> & layout, Pos pos = { 0.f, 0.f })
{
  if (layout->is<Rule>())
  {
    reader(std::static_pointer_cast<Rule>(layout), pos);
  }
  else if (box->isDerivedFrom<ListBox>())
  {
    auto listbox = std::static_pointer_cast<ListBox>(layout);
    if (listbox->is<HBox>())
    {
      read_hbox(reader, std::static_pointer_cast<HBox>(listbox), pos);
    }
    else
    {
      assert(listbox->is<VBox>());
      read_vbox(reader, std::static_pointer_cast<VBox>(listbox), pos);
    }
  }
  else
  {
    reader(layout, pos);
  }
}

} // namespace tex

#endif // LIBLAYOUT_LAYOUTREADER_H

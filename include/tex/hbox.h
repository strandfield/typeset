
#ifndef LIBLAYOUT_HBOX_H
#define LIBLAYOUT_HBOX_H

#include "tex/listbox.h"

namespace tex
{

class LIBLAYOUT_API HBox : public ListBox
{
public:
  HBox(List && list, float w, float h, float d, float shift = 0.f);
  ~HBox() = default;

};

LIBLAYOUT_API std::shared_ptr<HBox> hbox(List && hlist);
LIBLAYOUT_API std::shared_ptr<HBox> hbox(std::initializer_list<std::shared_ptr<Node>> && nodes);
LIBLAYOUT_API std::shared_ptr<HBox> hbox(List && hlist, float w);

} // namespace tex

#endif // LIBLAYOUT_HBOX_H

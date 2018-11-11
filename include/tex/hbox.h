
#ifndef LIBLAYOUT_HBOX_H
#define LIBLAYOUT_HBOX_H

#include "tex/listbox.h"

namespace tex
{

class LIBLAYOUT_API HBox : public ListBox
{
public:
  HBox(List && list, float w, float h, float d);
  ~HBox() = default;

};

} // namespace tex

#endif // LIBLAYOUT_HBOX_H

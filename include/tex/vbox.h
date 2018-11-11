
#ifndef LIBLAYOUT_VBOX_H
#define LIBLAYOUT_VBOX_H

#include "tex/listbox.h"

namespace tex
{

class LIBLAYOUT_API VBox : public ListBox
{
public:
  VBox(List && list, float w, float h, float d);
  ~VBox() = default;

};

} // namespace tex

#endif // LIBLAYOUT_VBOX_H

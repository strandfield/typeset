
#ifndef LIBLAYOUT_VBOX_H
#define LIBLAYOUT_VBOX_H

#include "tex/listbox.h"

namespace tex
{

class LIBLAYOUT_API VBox final : public ListBox
{
public:
  VBox(List && list, float w, float h, float d);
  ~VBox() = default;

};

LIBLAYOUT_API std::shared_ptr<VBox> vbox(List && list);
LIBLAYOUT_API std::shared_ptr<VBox> vtop(List && list);

} // namespace tex

#endif // LIBLAYOUT_VBOX_H

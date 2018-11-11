
#include "tex/vbox.h"

namespace tex
{

VBox::VBox(List && list, float w, float h, float d)
  : ListBox(std::move(list), w, h, d)
{

}

} // namespace tex


#include "tex/hbox.h"

namespace tex
{

HBox::HBox(List && list, float w, float h, float d)
  : ListBox(std::move(list), w, h, d)
{

}

} // namespace tex

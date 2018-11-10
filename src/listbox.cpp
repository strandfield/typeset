
#include "tex/listbox.h"

namespace tex
{

ListBox::ListBox(List && list, float w, float h, float d)
  : mList(std::move(list))
  , mWidth(w)
  , mHeight(h)
  , mDepth(d)
{

}

} // namespace tex

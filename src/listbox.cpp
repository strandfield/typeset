
#include "tex/listbox.h"

namespace tex
{

ListBox::ListBox(List && list, float w, float h, float d, float shift)
  : mList(std::move(list))
  , mWidth(w)
  , mHeight(h)
  , mDepth(d)
  , mShiftAmount(shift)
  , mGlueSettings{0.f, GlueOrder::Normal}
{

}

void ListBox::setGlue(float ratio, GlueOrder order)
{
  mGlueSettings.ratio = ratio;
  mGlueSettings.order = order;
}

} // namespace tex

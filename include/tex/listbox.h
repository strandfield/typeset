
#ifndef LIBLAYOUT_LISTBOX_H
#define LIBLAYOUT_LISTBOX_H

#include "tex/box.h"

#include "tex/glue.h"

#include <list>
#include <memory>

namespace tex
{

typedef std::list<std::shared_ptr<Node>> List;

class LIBLAYOUT_API ListBox : public Box
{
public:
  ListBox(List && list, float w, float h, float d, float shift = 0.f);
  ~ListBox() = default;

  inline float height() const override { return mHeight; }
  inline float depth() const override { return mDepth; }
  inline float width() const override { return mWidth; }
  inline void setHeight(float h) { mHeight = h; }
  inline void setDepth(float d) { mDepth = d; }
  inline void setWidth(float w) { mWidth = w; }

  inline float shiftAmount() const { return mShiftAmount; }

  inline List & list() { return mList; }
  inline const List & list() const { return mList; }

  inline float glueRatio() const { return mGlueSettings.ratio; }
  inline GlueOrder glueOrder() const { return mGlueSettings.order; }
  void setGlue(float ratio, GlueOrder order); 

private:
  List mList;
  float mWidth;
  float mHeight;
  float mDepth;
  float mShiftAmount;
  GlueSettings mGlueSettings;
};

} // namespace tex

#endif // LIBLAYOUT_LISTBOX_H

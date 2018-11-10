
#include "tex/node.h"

#include "tex/box.h"
#include "tex/glue.h"
#include "tex/kern.h"

namespace tex
{

bool Node::isBox() const
{
  return isDerivedFrom<Box>();
}

bool Node::isGlue() const
{
  return is<Glue>();
}

bool Node::isKern() const
{
  return is<Kern>();
}

} // namespace tex

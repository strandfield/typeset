
#include "tex/node.h"

#include "tex/glue.h"

namespace tex
{

bool Node::isGlue() const
{
  return is<Glue>();
}

} // namespace tex

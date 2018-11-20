
#include "tex/penalty.h"

namespace tex
{

Penalty::Penalty(int val)
  : mValue(val)
{

}

std::shared_ptr<Penalty> penalty(int p)
{
  return std::make_shared<Penalty>(p);
}

std::shared_ptr<Penalty> infinitePenalty()
{
  static const std::shared_ptr<Penalty> p = penalty(Penalty::Infinity);
  return p;
}

} // namespace tex

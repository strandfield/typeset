
#include "tex/kern.h"

namespace tex
{

Kern::Kern(float s)
  : mSpace(s)
{

}

std::shared_ptr<Kern> kern(float space)
{
  return std::make_shared<Kern>(space);
}

} // namespace tex


#ifndef LIBLAYOUT_KERN_H
#define LIBLAYOUT_KERN_H

#include "tex/node.h"

namespace tex
{

class LIBLAYOUT_API Kern final : public Node
{
public:
  explicit Kern(float value);
  ~Kern() = default;

  inline float space() const { return mSpace; }

private:
  float mSpace;
};

} // namespace tex

#endif // LIBLAYOUT_KERN_H

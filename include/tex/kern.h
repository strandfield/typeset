
#ifndef LIBLAYOUT_KERN_H
#define LIBLAYOUT_KERN_H

#include "tex/node.h"

#include <memory>

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

LIBLAYOUT_API std::shared_ptr<Kern> kern(float space);

} // namespace tex

#endif // LIBLAYOUT_KERN_H

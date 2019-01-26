// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_KERN_H
#define LIBTYPESET_KERN_H

#include "tex/node.h"

#include <memory>

namespace tex
{

class LIBTYPESET_API Kern final : public Node
{
public:
  explicit Kern(float value);
  ~Kern() = default;

  inline float space() const { return mSpace; }

private:
  float mSpace;
};

LIBTYPESET_API std::shared_ptr<Kern> kern(float space);

} // namespace tex

#endif // LIBTYPESET_KERN_H

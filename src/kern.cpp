// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

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

// Copyright (C) 2018 Vincent Chambrin
// This file is part of the liblayout project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBLAYOUT_NODE_H
#define LIBLAYOUT_NODE_H

#include "tex/defs.h"

#include <memory>
#include <typeinfo>

namespace tex
{

class LIBLAYOUT_API Node
{
public:
  Node() = default;
  Node(const Node &) = delete;
  virtual ~Node() = default;

  template<typename T>
  bool is() const
  {
    return typeid(T).hash_code() == typeid(*this).hash_code();
  }

  template<typename T>
  bool isDerivedFrom() const
  {
    return dynamic_cast<const T*>(this) != nullptr;
  }

  template<typename T>
  T & as()
  {
    return *static_cast<T*>(this);
  }

  template<typename T>
  const T & as() const
  {
    return *static_cast<const T*>(this);
  }

  bool isBox() const;
  bool isGlue() const;
  bool isKern() const;
  bool isPenalty() const;
};

template<typename T, typename U = Node>
std::shared_ptr<T> cast(const std::shared_ptr<U> & node)
{
  return std::static_pointer_cast<T>(node);
}

} // namespace tex

#endif // LIBLAYOUT_NODE_H

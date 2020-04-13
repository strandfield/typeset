// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_NODE_H
#define LIBTYPESET_NODE_H

#include "tex/defs.h"

#include <memory>
#include <typeinfo>

namespace tex
{

class LIBTYPESET_API Node
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
  virtual bool isCharacterBox() const;
  virtual bool isHBox() const;
  virtual bool isVBox() const;
  bool isListBox() const;
  virtual bool isMathSymbol() const;
};

template<typename T, typename U = Node>
std::shared_ptr<T> cast(const std::shared_ptr<U> & node)
{
  return std::static_pointer_cast<T>(node);
}

} // namespace tex

#endif // LIBTYPESET_NODE_H

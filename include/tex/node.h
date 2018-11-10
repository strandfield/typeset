
#ifndef LIBLAYOUT_NODE_H
#define LIBLAYOUT_NODE_H

#include "tex/defs.h"

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

  bool isGlue() const;
};

} // namespace tex

#endif // LIBLAYOUT_NODE_H

#pragma once

#include "Node.h"

namespace nTiled {
namespace pipeline {
namespace hierarchical {
namespace lo {

class Parent {
public:
  /*! @brief Set subnode of index child->getIndex() to child 
   */
  virtual void updateChild(Node* child) = 0;
};


}
}
}
}

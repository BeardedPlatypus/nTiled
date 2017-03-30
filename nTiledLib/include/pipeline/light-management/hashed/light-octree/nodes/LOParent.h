#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "LONode.h"


namespace nTiled {
namespace pipeline {
namespace hashed {

class LOParent {
public:
  /*! @brief update the child at the given index with the specified new child
   *
   * @param index The index of the child which should be updated
   * @param child Pointer to the new child index which should be updated
   */
  virtual void updateChild(glm::bvec3 index, LONode* child) = 0;
};

}
}
}

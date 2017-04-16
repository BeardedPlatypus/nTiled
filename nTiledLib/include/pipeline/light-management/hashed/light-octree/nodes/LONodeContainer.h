#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "LONode.h"
#include "LOParent.h"


namespace nTiled {
namespace pipeline {
namespace hashed {

struct LONodeContainer {
  /*! @brief Construct a new LONodeContainer with the given 
   *         node and parent of this node.
   * 
   * @param p_node pointer to the node of this LONodeContainer
   * @param p_parent pointer to the parent of this LONode
   */
  LONodeContainer(LONode* p_node,
                  LOParent* p_parent,
                  glm::bvec3 index);

  /*! @brief Pointer to the node of this LONodeContainer. */
  LONode* p_node;

  /*! @brief Pointer to parent of this LONodeContainer's p_node. */
  LOParent* p_parent;

  /*! @brief index the node of this LONodeContainer has in parent. */
  glm::bvec3 index;
};


}
}
}

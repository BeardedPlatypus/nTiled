#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glm\glm.hpp>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "SLTNode.h"

namespace nTiled {
namespace pipeline {
namespace hashed {

/*! @brief SingleLightTree leaf node.
 */
class SLTBranch : public SLTNode {
public:
  /*! @brief Create a new empty Branch node
   */
  SLTBranch();

  ~SLTBranch();

  /*! @brief Get the child SLTNode at the specified position
   * 
   * @param position The position in boolean coordinates of the child to 
   *                 be retrieved.
   *
   * @return The child at the specified position
   */
  SLTNode* getChildNode(glm::bvec3 position) const;

  /*! @brief Set the node at position to new_node
   * 
   * @param position The position to which new_node should be set
   * @param new_node The pointer to the new node
   *
   * @post | (new this)->getChildNode(position) == new_node
   */
  void setNode(glm::bvec3 position, SLTNode* new_node);


  // --------------------------------------------------------------------------
  virtual bool isInLight(glm::vec3 point,
                         NodeDimensions dim) const;

  // --------------------------------------------------------------------------
  //  LightOctree construction methods
  // --------------------------------------------------------------------------
  virtual void addToLeaf(LOLeaf* p_leaf, 
                         LOParent* p_parent,
                         glm::bvec3 node_index,
                         GLuint index) const;

  virtual void addToBranch(LOBranch* p_branch, 
                           LOParent* p_parent,
                           glm::bvec3 node_index,
                           GLuint index) const;
private:
  /*! @brief The children of this Branch node. */
  SLTNode* children[8];

  /*! @brief Whether any of the children is set.*/
  bool has_set[8];
};

}
}
}
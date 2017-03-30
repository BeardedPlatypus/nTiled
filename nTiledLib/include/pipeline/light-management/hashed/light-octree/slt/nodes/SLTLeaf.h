#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "SLTNode.h"
#include "pipeline\light-management\hashed\light-octree\slt\NodeType.h"

namespace nTiled {
namespace pipeline {
namespace hashed {

/*! @brief SingleLightTree leaf node. 
 */
class SLTLeaf : public SLTNode {
public:
  /*! @brief Construct a new Leaf node that has_light. 
   *
   * @param whether this new Leaf node contains (a portion of) the light.
   */
  SLTLeaf(bool has_light);

  /*! @brief Construct a new Leaf node that is of NodeType::Filled or NodeType::Empty
   * 
   * @param node_type The NodeType of this new SLTLeaf
   */
  SLTLeaf(NodeType node_type);

  /*@ brief Whether this Leaf node contains (a portion of) the light 
   * 
   * @return Whether this Leaf node contains (a portion of) the light.
   */
  bool hasLight() const { return this->has_light; }


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
  /*! @brief whether this Leaf node contains (a portion of) the light. */
  bool has_light;
};

}
}
}
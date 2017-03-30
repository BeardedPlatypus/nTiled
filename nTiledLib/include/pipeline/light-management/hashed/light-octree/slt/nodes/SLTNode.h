#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glad\glad.h>

// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "pipeline\light-management\hashed\light-octree\NodeDimensions.h"


namespace nTiled {
namespace pipeline {
namespace hashed {

class LOLeaf;
class LOBranch;
class LOParent;

class SLTNode {
public:
  virtual ~SLTNode() {}

  /*! @brief Query if this point falls within a light node
   *
   * @param point The point being queried
   * @param dim The SLTNodeDimensions of this node
   */
  virtual bool isInLight(glm::vec3 point,
                         NodeDimensions dim) const = 0;

  // --------------------------------------------------------------------------
  //  LightOctree construction methods
  // --------------------------------------------------------------------------
  /*! @brief Add this SLTNode to the specified LOLeaf node
   *    
   * @param p_leaf The LightOctree node to which this SLTNode should be added
   */
  virtual void addToLeaf(LOLeaf* p_leaf, 
                         LOParent* p_parent,
                         glm::bvec3 node_index,
                         GLuint index) const = 0;

  /*! @brief Add this SLTNode to the specified LOBranch node
   *    
   * @param p_branch The LightOctree node to which this SLTNode should be added
   */
  virtual void addToBranch(LOBranch* p_branch, 
                           LOParent* p_parent,
                           glm::bvec3 node_index,
                           GLuint index) const = 0;
};

}
}
}
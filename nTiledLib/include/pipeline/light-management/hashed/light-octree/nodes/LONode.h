#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glad\glad.h>
#include <vector>

// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "pipeline\light-management\hashed\light-octree\slt\nodes\SLTNode.h"

namespace nTiled {
namespace pipeline {
namespace hashed {

class LOParent;
struct LONodeContainer;

class LONode {
public:
  virtual ~LONode() {}

  // --------------------------------------------------------------------------
  //  Construction methods
  // --------------------------------------------------------------------------
  /*! @brief Add the specified SLTNode to this LONode
   * 
   * @param node constant reference to the SLTNode to be added to this LONode.
   */
  virtual void addSLTNode(const SLTNode& node, 
                          LOParent* p_parent,
                          glm::bvec3 node_index,
                          GLuint index) = 0;

  /*! @brief Retrieve and construct the root node at once depth_left has been
   *         reached, approximating mid_point_slt. Return 
   * 
   * @param depth_left The number of layers left to descend into
   * @param mid_point_slt The midpoint of the SingleLightTree of which the root
   *                      node should be retrievied.
   * @param node_dim The node dimensions corresponding with this node.
   * @param p_parent pointer to the parent of this node.
   */
  virtual LONodeContainer retrieveAndConstructRoot(unsigned int depth_left,
                                                   glm::vec3 mid_point_slt,
                                                   NodeDimensions node_dim,
                                                   LOParent* p_parent,
                                                   glm::bvec3 index) = 0;

  // --------------------------------------------------------------------------
  //  Query methods
  // --------------------------------------------------------------------------
  /*! @brief Retrieve the light indices associated with the provided point 
   *         from this node.
   *
   * @param point The point of which the light indices should be returned
   * @param node The node dimensions of this LONode
   * 
   * @returns The light indices associated with the provided point.
   */
  virtual std::vector<GLuint> retrieveLights(glm::vec3 point,
                                             NodeDimensions node) const = 0;
};

}
}
}
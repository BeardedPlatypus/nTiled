#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <vector>
#include "glad\glad.h"

// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "LONode.h"


namespace nTiled {
namespace pipeline {
namespace hashed {

class LOBranch;

class LOLeaf : public LONode {
public:
  // --------------------------------------------------------------------------
  //  Constructor 
  // --------------------------------------------------------------------------
  /*! @brief Construct a new empty LOLeaf node. */
  LOLeaf();

  /*! @brief Construct a new LOLeaf with a copy of the contents of node_ref
   *
   * @param node_ref The LOLeaf which is being copied into this new LOLeaf
   */
  LOLeaf(const LOLeaf& node_ref);

  // --------------------------------------------------------------------------
  //  Get methods
  // --------------------------------------------------------------------------
  virtual std::vector<GLuint> retrieveLights(glm::vec3 point,
                                             NodeDimensions node) const;

  /*! @brief Return whether this node is empty
   * 
   * @returns True if this node is empty, false otherwise.
   */
  bool isEmpty() const { return this->indices.empty(); }


  /*! @brief Return the indices in this LOLeaf node
   *
   * @returns The indices in this LOLeaf node
   */
  std::vector<GLuint> getIndices() const { return this->indices; }

  // --------------------------------------------------------------------------
  //  Construction methods
  // --------------------------------------------------------------------------
  virtual void addSLTNode(const SLTNode& node, 
                          LOParent* p_parent,
                          glm::bvec3 node_index,
                          GLuint index);

  virtual LONodeContainer retrieveAndConstructRoot(unsigned int depth_left,
                                                   glm::vec3 mid_point_slt,
                                                   NodeDimensions node_dim,
                                                   LOParent* p_parent,
                                                   glm::bvec3 index);

  /*! @brief Subdivide this node into a LOBranch node with 8 child nodes equal
   *         to this node.
   * 
   * @param parent The parent node of this node
   * @param index The index of this node within the specified parent
   *
   * @return The constructed new LOBranch node
   */
  LOBranch* subdivide(LOParent* parent, glm::bvec3 index);


  /*! @brief Add the provided index to this LOLeaf node
   *
   * @param index The index to be added tho this LOLeaf
   *
   * @post | index IN (new this)->getIndices()
   */
  void addIndex(GLuint index);

private:
  /*! @brief The light indices associated with this LOLeaf. */
  std::vector<GLuint> indices;
};

}
}
}

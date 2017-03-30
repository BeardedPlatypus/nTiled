#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <vector>

// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "LONode.h"
#include "LOParent.h"


namespace nTiled {
namespace pipeline {
namespace hashed {

class LOLeaf;

class LOBranch : public LONode, public LOParent {
public:
  // --------------------------------------------------------------------------
  //  Constructor | Destructor
  // --------------------------------------------------------------------------
  /*! @brief Create a new empty LOBranch node
   */
  LOBranch();

  /*! @brief Create a new LOBranch node with children consisting of LeafNodes
   *         equal to the specified LOLeaf node
   * 
   * @param leaf The leaf node of which the data is copied.
   */
  LOBranch(LOLeaf* leaf);

  /*! @brief Destruct this LOBranch node.
   */
  ~LOBranch();

  // --------------------------------------------------------------------------
  //  Get Methods
  // --------------------------------------------------------------------------
  /*! @brief Get the child LONode at the specified position
   * 
   * @param position The position in boolean coordinates of the child to 
   *                 be retrieved.
   *
   * @return The child at the specified position
   */
  LONode* getChildNode(glm::bvec3 position);

  const LONode& getChildNodeConst(glm::bvec3 position) const;

  // --------------------------------------------------------------------------
  //  LONode methods
  // --------------------------------------------------------------------------
  virtual std::vector<GLuint> retrieveLights(glm::vec3 point,
                                             NodeDimensions node) const;

  virtual void addSLTNode(const SLTNode& node, 
                          LOParent* p_parent,
                          glm::bvec3 node_index,
                          GLuint index);

  virtual LONode* retrieveAndConstructRoot(unsigned int depth_left,
                                           glm::vec3 mid_point_slt,
                                           NodeDimensions node_dim,
                                           LOParent* p_parent,
                                           glm::bvec3 index);

  // --------------------------------------------------------------------------
  //  Parent methods
  // --------------------------------------------------------------------------
  virtual void updateChild(glm::bvec3 index, LONode* child);

private:
  /*! @brief The children nodes of this LOBranch node. */
  LONode* children[8];
};

}
}
}



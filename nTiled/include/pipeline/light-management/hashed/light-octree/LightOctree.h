#pragma once

// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "single-light-tree\SingleLightTree.h"
#include "nodes\Node.h"
#include "nodes\Root.h"


namespace nTiled {
namespace pipeline {
namespace hashed {

class LightOctree {
public:
  // --------------------------------------------------------------------------
  //  Constructor | Destructor
  // --------------------------------------------------------------------------
  LightOctree(glm::vec4 octree_origin,
              float minimum_leaf_node_size,
              unsigned int octree_depth);
  ~LightOctree();

  // --------------------------------------------------------------------------
  //  Building Methods
  // --------------------------------------------------------------------------
  /*! @brief Add the specified SingleLightTree to this LightOctree
  */
  void addSLT(const SingleLightTree& slt);

  // --------------------------------------------------------------------------
  //  Get methods
  // --------------------------------------------------------------------------
  /*! @brief Get the root node of this LightOctree
  *
  * @return The root of this LightOctree
  */
  const lo::Node& getRoot() const { return *this->p_root; }

  /*! @brief Get the origin of this LightOctree
  *
  * @return The origin in homogeneous coordinates of this LightOctree
  */
  const glm::vec4 getOrigin() const { return this->octree_origin; }

  /*! @brief Get the minimum node size of this LightOctree
  *
  * @return The minmimum leaf node size of this LightOctree
  */
  const float getMinimumLeafNodeSize() const { return this->minimum_leaf_node_size; }

  /*! @brief Get the depth f this LightOctree
  *
  * @return the depth of this LightOctree
  */
  const unsigned int getDepth() const { return this->octree_depth; }

  /*! @brief Get all the nodes at layer_i of this LightOctree
  *
  * @param layer_i The layer of which the nodes are obtained
  *
  * @return The nodes at layer_i of this LightOctree
  */
  std::vector<std::pair<glm::uvec3, lo::Node*>> getNodesAtDepth(unsigned int layer_i);

private:
  /*! @brief Root lo::Node of this LightOctree. */
  lo::Root* p_root;

  /*! @brief The origin of this LightOctree. */
  const glm::vec4 octree_origin;

  /*! @brief Minimum node size of this LightOctree */
  const float minimum_leaf_node_size;

  /*! @brief Depth of this LightOctree */
  const unsigned int octree_depth;
};

}
}
}
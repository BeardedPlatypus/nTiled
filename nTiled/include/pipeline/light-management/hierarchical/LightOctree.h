#pragma once

// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "single-light-tree\SingleLightTree.h"
#include "light-octree\Node.h"
#include "light-octree\Root.h"


namespace nTiled {
namespace pipeline {
namespace hierarchical {

class LightOctree {
public:
  // --------------------------------------------------------------------------
  //  Constructor | Destructor
  // --------------------------------------------------------------------------
  LightOctree(glm::vec4 octree_origin, 
              float minimum_leaf_node_size, 
              unsigned int octree_depth);
  ~LightOctree();

  void addSLT(const SingleLightTree& slt);

  const lo::Node& getRoot() const { return *this->p_root; }
  const glm::vec4 getOrigin() const { return this->octree_origin; }
  const float getMinimumLeafNodeSize() const { return this->minimum_leaf_node_size; }
  const unsigned int getDepth() const { return this->octree_depth; }

private:
  /*! @brief Root lo::Node of this LightOctree. */
  lo::Root* p_root;

  const glm::vec4 octree_origin;
  const float minimum_leaf_node_size;
  const unsigned int octree_depth;
};

}
}
}

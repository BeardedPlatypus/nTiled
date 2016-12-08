#include "pipeline\light-management\hierarchical\LightOctree.h"

namespace nTiled {
namespace pipeline {
namespace hierarchical {

LightOctree::LightOctree(glm::vec4 octree_origin,
                         float minimum_leaf_node_size,
                         unsigned int octree_depth) :
  octree_origin(octree_origin),
  minimum_leaf_node_size(minimum_leaf_node_size),
  octree_depth(octree_depth),
  p_root(new lo::Root()) { }


LightOctree::~LightOctree() {
  delete this->p_root;
}


void LightOctree::addSLT(const SingleLightTree& slt) {
  this->p_root->addSLT(slt, 
                       this->octree_depth, 
                       glm::uvec3(1 << (this->octree_depth - 1)));
}


} // hierarchical
} // pipeline
} // nTiled

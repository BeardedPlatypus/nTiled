#include "pipeline\light-management\hierarchical\LinklessOctree.h"

#include <cmath>

namespace nTiled {
namespace pipeline {
namespace hierarchical {


LinklessOctree::LinklessOctree() { }

void LinklessOctree::fromLightOctree(LightOctree* p_light_octree) {
  // get new depth octree
  unsigned int depth = p_light_octree->getDepth() - 2;
  if (depth < 1) depth = 1;

  // initialise containing vector
  this->leaf_hash_maps = std::vector<SpatialHashFunction<glm::uvec2>>(depth);
  this->has_leaf_hash_map = std::vector<bool>(depth, false);
  this->node_hash_maps = std::vector<SpatialHashFunction<glm::bvec2>>(depth);

  // node structures
  std::vector<std::pair<glm::uvec3, lo::Node*>> partial_nodes;
  std::vector<std::pair<glm::uvec3, lo::Node*>> leaf_nodes;

  // build initial nodes

  // build each level of SpatialHashFunction
  for (int i = 0; i < depth; i++) {
    
  }

}



}
}
}

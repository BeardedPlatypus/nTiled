#include "pipeline\light-management\hierarchical\single-light-tree\SLTBuilder.h"

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <queue>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "math\util.h"

namespace nTiled {
namespace pipeline {
namespace hierarchical {

SLTBuilder::SLTBuilder(const float minimum_leaf_node_size,
                       const glm::vec4 origin_octree) : 
  minimum_leaf_node_size(minimum_leaf_node_size),
  origin_octree(glm::vec3(origin_octree) / origin_octree.w) { }


SingleLightTree* SLTBuilder::buildSingleLightTree(const world::PointLight& light,
                                                  GLuint index) {
  slt::NoLightNode* no_light = new slt::NoLightNode();
  slt::FullLightNode* full_light = new slt::FullLightNode();

  slt::Lattice* current_lattice = this->lightToLattice(light, no_light, full_light);

  slt::Lattice* next_lattice;
  
  std::vector<slt::PartialLightNode const *> partial_light_nodes =
    std::vector<slt::PartialLightNode const *>();

  while (current_lattice->getNNodes() > 1) {
    next_lattice = combineLatticeStep(*current_lattice,
                                      no_light,
                                      partial_light_nodes);

    delete current_lattice;
    current_lattice = next_lattice;
  }

  glm::uvec3 middle_slt = (current_lattice->getOriginInOctree() + 
                           glm::uvec3(1 << (current_lattice->getDepth() - 2)));

  return new SingleLightTree(light,
                             index,
                             current_lattice->getOctreeOffset(),
                             current_lattice->getOriginInOctree(),
                             middle_slt,
                             *current_lattice->getLatticeNode(glm::uvec3(0)).node,
                             current_lattice->getDepth(),
                             current_lattice->getMinimumNodeSize(),
                             full_light,
                             no_light,
                             partial_light_nodes);
}


} // hierarchical
} // pipeline
} // nTiled
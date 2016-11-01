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

SLTBuilder::SLTBuilder(const float minimum_leaf_node_size) : 
  minimum_leaf_node_size(minimum_leaf_node_size) { }


SingleLightTree* SLTBuilder::buildSingleLightTree(const world::PointLight& light) {
  slt::NoLightNode* no_light = new slt::NoLightNode();
  slt::FullLightNode* full_light = new slt::FullLightNode();

  slt::Lattice* current_lattice = this->lightToLattice(light, no_light, full_light);
  slt::Lattice* next_lattice;
  
  std::vector<slt::PartialLightNode const * const> partial_light_nodes =
    std::vector<slt::PartialLightNode const * const>();

  while (current_lattice->getNNodes() > 1) {
    next_lattice = combineLatticeStep(*current_lattice,
                                      no_light,
                                      partial_light_nodes);

    delete current_lattice;
    current_lattice = next_lattice;
  }

  return new SingleLightTree(light,
                             current_lattice->getOriginInLattice(),
                             *current_lattice->getLatticeNode(glm::uvec3(0)).node,
                             current_lattice->getNodeSize(),
                             full_light,
                             no_light,
                             partial_light_nodes);
}


} // hierarchical
} // pipeline
} // nTiled
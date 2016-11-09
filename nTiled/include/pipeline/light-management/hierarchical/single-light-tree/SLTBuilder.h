#pragma once

// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "pipeline\light-management\hierarchical\single-light-tree\SingleLightTree.h"
#include "pipeline\light-management\hierarchical\single-light-tree\construction-util\Offsets.h"
#include "pipeline\light-management\hierarchical\single-light-tree\construction-util\Lattice.h"
#include "pipeline\light-management\hierarchical\single-light-tree\SingleLightTreeNodes.h"

namespace nTiled {
namespace pipeline {
namespace hierarchical {

class SLTBuilder {
public:
  SLTBuilder(const float minimum_leaf_node_size,
             const glm::vec4 origin_lattice);

  SingleLightTree* buildSingleLightTree(const world::PointLight& light);

private:
  // --------------------------------------------------------------------------
  //  SLT construction methods
  // --------------------------------------------------------------------------
  // Phase 1
  bool lightWithinNode(const world::PointLight& light,
                       glm::vec4 octree_node_origin,
                       float slt_node_edge);

  slt::Lattice* lightToLattice(const world::PointLight& light,
                             slt::NoLightNode const * const no_light,
                             slt::FullLightNode const * const full_light);

  // --------------------------------------------------------------------------
  // Phase 2
  slt::Lattice* combineLatticeStep(const slt::Lattice& lattice,
                                   slt::NoLightNode const * const no_light,
                                   std::vector<slt::PartialLightNode const *>& partial_light_nodes);

  slt::Offsets calculateOffsets(const slt::Lattice& lattice);

  void updateCoreLattice(const slt::Lattice& current_lattice,
                         const slt::Offsets& offsets,
                         slt::Lattice* next_lattice,
                         std::vector<slt::PartialLightNode const *>& partial_light_nodes);

  void updateSidesLattice(const slt::Lattice& current_lattice,
                          const slt::Offsets& offsets,
                          slt::NoLightNode const * const no_light,
                          slt::Lattice* next_lattice,
                          std::vector<slt::PartialLightNode const *>& partial_light_nodes);

  void updateEdgesLattice(const slt::Lattice& current_lattice,
                          const slt::Offsets& offsets,
                          slt::NoLightNode const * const no_light,
                          slt::Lattice* next_lattice,
                          std::vector<slt::PartialLightNode const *>& partial_light_nodes);

  void updateCornersLattice(const slt::Lattice& current_lattice,
                            const slt::Offsets& offsets,
                            slt::NoLightNode const * const no_light,
                            slt::Lattice* next_lattice,
                            std::vector<slt::PartialLightNode const *>& partial_light_nodes);

  // --------------------------------------------------------------------------
  const float minimum_leaf_node_size;
  const glm::vec4 origin_lattice;
};

} // hierarchical
} // pipeline
} // nTiled
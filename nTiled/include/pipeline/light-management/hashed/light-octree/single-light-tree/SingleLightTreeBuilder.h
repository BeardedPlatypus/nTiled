#pragma once

// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "SingleLightTree.h"
#include "octree-layer\Offsets.h"
#include "octree-layer\OctreeLayer.h"


namespace nTiled {
namespace pipeline {
namespace hashed {

class SingleLightTreeBuilder {
public:
  SingleLightTreeBuilder(const float minimum_leaf_node_size,
                         const glm::vec4 origin_octree,
                         const unsigned int octree_depth);

  SingleLightTree* buildSingleLightTree(const world::PointLight& light,
                                        GLuint index);

private:
  // --------------------------------------------------------------------------
  //  SLT construction methods
  // --------------------------------------------------------------------------
  // Phase 1
  bool lightWithinNode(const world::PointLight& light,
                       glm::vec4 octree_node_origin,
                       float slt_node_edge);

  slt::OctreeLayer* lightToLattice(const world::PointLight& light,
                                   slt::No const * const no_light,
                                   slt::Full const * const full_light);

  // --------------------------------------------------------------------------
  // Phase 2
  slt::OctreeLayer* combineLatticeStep(const slt::OctreeLayer& lattice,
                                       slt::No const * const no_light,
                                       std::vector<slt::Partial const *>& partial_light_nodes);

  slt::Offsets calculateOffsets(const slt::OctreeLayer& lattice);

  void updateCoreLattice(const slt::OctreeLayer& current_lattice,
                         const slt::Offsets& offsets,
                         slt::OctreeLayer* next_lattice,
                         std::vector<slt::Partial const *>& partial_light_nodes);

  void updateSidesLattice(const slt::OctreeLayer& current_lattice,
                          const slt::Offsets& offsets,
                          slt::No const * const no_light,
                          slt::OctreeLayer* next_lattice,
                          std::vector<slt::Partial const *>& partial_light_nodes);

  void updateEdgesLattice(const slt::OctreeLayer& current_lattice,
                          const slt::Offsets& offsets,
                          slt::No const * const no_light,
                          slt::OctreeLayer* next_lattice,
                          std::vector<slt::Partial const *>& partial_light_nodes);

  void updateCornersLattice(const slt::OctreeLayer& current_lattice,
                            const slt::Offsets& offsets,
                            slt::No const * const no_light,
                            slt::OctreeLayer* next_lattice,
                            std::vector<slt::Partial const *>& partial_light_nodes);

  // --------------------------------------------------------------------------
  /*! @brief The size of the smallest nodes of octree layer octree_depth - 1. */
  const float minimum_leaf_node_size;

  /*! @brief The origin of the octree in which the lights are constructed. */
  const glm::vec3 origin_octree;

  /*! @brief The number of layers that exist in this octree. */
  const unsigned int octree_depth;
};

} // hierarchical
} // pipeline
} // nTiled


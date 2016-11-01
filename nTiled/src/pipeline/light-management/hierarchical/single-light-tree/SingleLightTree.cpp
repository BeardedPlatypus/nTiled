#include "pipeline\light-management\hierarchical\single-light-tree\SingleLightTree.h"

namespace nTiled {
namespace pipeline {
namespace hierarchical {

SingleLightTree::SingleLightTree(const world::PointLight& light,
                                 const glm::ivec3 position,
                                 const slt::Node& root,
                                 const float root_size,
                                 slt::FullLightNode const * const p_full_light,
                                 slt::NoLightNode const * const p_no_light,
                                 const std::vector<slt::PartialLightNode const * const> partial_light_nodes) :
    light(light),
    position(position),
    root(root),
    root_size(root_size),
    p_full_light(p_full_light),
    p_no_light(p_no_light),
    partial_light_nodes(partial_light_nodes) { }


SingleLightTree::~SingleLightTree() {
  delete p_full_light;
  delete p_no_light;

  for (slt::PartialLightNode const * const p_partial_node : this->partial_light_nodes) {
    delete p_partial_node;
  }
}


} // hierarchical
} // pipeline
} // nTiled
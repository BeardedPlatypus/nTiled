#include "pipeline\light-management\hierarchical\single-light-tree\SingleLightTreeNodes.h"

namespace nTiled {
namespace pipeline {
namespace hierarchical {
namespace slt {

// ----------------------------------------------------------------------------
FullLightNode::FullLightNode() {}
FullLightNode::~FullLightNode() {}

// ----------------------------------------------------------------------------
NoLightNode::NoLightNode() {}
NoLightNode::~NoLightNode() {}

// ----------------------------------------------------------------------------
PartialLightNode::PartialLightNode(const Node& child_000,
                                   const Node& child_100,
                                   const Node& child_010,
                                   const Node& child_110,
                                   const Node& child_001,
                                   const Node& child_101,
                                   const Node& child_011,
                                   const Node& child_111) :
  child_000(child_000),
  child_001(child_001),
  child_010(child_010),
  child_011(child_011),
  child_100(child_100),
  child_101(child_101),
  child_110(child_110),
  child_111(child_111) {
}


PartialLightNode::PartialLightNode(Node const * const * children) :
  PartialLightNode(*(children[0]),
                   *(children[1]),
                   *(children[2]),
                   *(children[3]),
                   *(children[4]),
                   *(children[5]),
                   *(children[6]),
                   *(children[7])) {
}

} // slt
} // hierarchical
} // pipeline
} // nTiled
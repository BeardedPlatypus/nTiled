#include "pipeline\light-management\hierarchical\single-light-tree\construction-util\LatticeNode.h"

namespace nTiled {
namespace pipeline {
namespace hierarchical {
namespace slt {

LatticeNode::LatticeNode(const NodeType type,
                               Node const * const node) :
  type(type),
  node(node) { }

LatticeNode::LatticeNode() : LatticeNode(NodeType::Undetermined,
                                         nullptr) { }

LatticeNode::LatticeNode(FullLightNode const * const node) :
  LatticeNode(NodeType::FullLight, node) { }

LatticeNode::LatticeNode(NoLightNode const * const node) :
  LatticeNode(NodeType::NoLight, node) { }

LatticeNode::LatticeNode(PartialLightNode const * const node) :
  LatticeNode(NodeType::PartialLight, node) { }

} // slt
} // hierarchical
} // pipeline
} // nTiled
#pragma once

#include "pipeline\light-management\hierarchical\single-light-tree\SingleLightTreeNodes.h"

namespace nTiled {
namespace pipeline {
namespace hierarchical {
namespace slt {

enum class NodeType {
  NoLight,
  FullLight,
  PartialLight,
  Undetermined
};


struct LatticeNode {
public:
  LatticeNode();
  LatticeNode(const NodeType type,
              Node const * const node);
  LatticeNode(FullLightNode const * const node);
  LatticeNode(NoLightNode const * const node);
  LatticeNode(PartialLightNode const * const node);

  const NodeType type;
  Node const * const node;
};


} // slt
} // hierarchical
} // pipeline
} // nTiled

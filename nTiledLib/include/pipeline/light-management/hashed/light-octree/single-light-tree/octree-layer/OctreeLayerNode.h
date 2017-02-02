#pragma once

#include "pipeline\light-management\hashed\light-octree\single-light-tree\nodes\Full.h"
#include "pipeline\light-management\hashed\light-octree\single-light-tree\nodes\No.h"
#include "pipeline\light-management\hashed\light-octree\single-light-tree\nodes\Partial.h"

namespace nTiled {
namespace pipeline {
namespace hashed {
namespace slt {

enum class NodeType {
  NoLight,
  FullLight,
  PartialLight,
  Undetermined
};


struct OctreeLayerNode{
public:
  OctreeLayerNode();
  OctreeLayerNode(const NodeType type,
                  Node const * const node);
  OctreeLayerNode(Full const * const node);
  OctreeLayerNode(No const * const node);
  OctreeLayerNode(Partial const * const node);

  const NodeType type;
  Node const * const node;
};


} // slt
} // hierarchical
} // pipeline
} // nTiled

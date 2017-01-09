#include "pipeline\light-management\hashed\light-octree\single-light-tree\octree-layer\OctreeLayerNode.h"

namespace nTiled {
namespace pipeline {
namespace hashed {
namespace slt {

OctreeLayerNode::OctreeLayerNode(const NodeType type,
                                 Node const * const node) :
  type(type),
  node(node) {
}

OctreeLayerNode::OctreeLayerNode() : OctreeLayerNode(NodeType::Undetermined,
                                                     nullptr) {
}

OctreeLayerNode::OctreeLayerNode(Full const * const node) :  
    OctreeLayerNode(NodeType::FullLight, node) {
}

OctreeLayerNode::OctreeLayerNode(No const * const node) :
    OctreeLayerNode(NodeType::NoLight, node) {
}

OctreeLayerNode::OctreeLayerNode(Partial const * const node) :
    OctreeLayerNode(NodeType::PartialLight, node) {
}

} // slt
} // hashed
} // pipeline
} // nTiled

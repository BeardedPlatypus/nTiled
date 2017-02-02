#include "pipeline\light-management\hashed\light-octree\nodes\Node.h"

// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "pipeline\light-management\hashed\light-octree\nodes\Parent.h"

namespace nTiled {
namespace pipeline {
namespace hashed {
namespace lo {

Node::Node(Parent* p_parent, unsigned char index) : p_parent(p_parent),
index(index) {
}


void Node::addSLT(unsigned int current_depth,
                  glm::uvec3 current_middle,
                  const SingleLightTree& slt) {
  if (current_depth == slt.getDepth()) {
    this->addSLTNode(slt, slt.getRoot());
  } else {
    this->addSLTInternal(current_depth, current_middle, slt);
  }
}

} // lo
} // hierarchical
} // pipeline
} // nTiled

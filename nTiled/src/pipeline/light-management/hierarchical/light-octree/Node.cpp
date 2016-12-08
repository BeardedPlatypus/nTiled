#include "pipeline\light-management\hierarchical\light-octree\Node.h"

// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "pipeline\light-management\hierarchical\light-octree\Parent.h"

namespace nTiled {
namespace pipeline {
namespace hierarchical {
namespace lo {

Node::Node(Parent* p_parent, unsigned char index) : p_parent(p_parent),
                                                    index(index) { }


void Node::addSLT(const SingleLightTree& slt,
                  unsigned int depth,
                  glm::uvec3 current_middle) {
  if (depth == slt.getDepth()) {
    this->addSLTNode(slt, slt.getRoot());
  } else {
    this->addSLTInternal(slt, depth, current_middle);
  }
}

} // lo
} // hierarchical
} // pipeline
} // nTiled

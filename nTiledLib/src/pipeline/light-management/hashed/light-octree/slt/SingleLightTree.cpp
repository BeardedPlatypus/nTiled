#include "pipeline\light-management\hashed\light-octree\slt\SingleLightTree.h"


namespace nTiled {
namespace pipeline {
namespace hashed {

// --------------------------------------------------------------------------
//  Constructor | Destructor
// --------------------------------------------------------------------------
SingleLightTree::SingleLightTree(glm::vec3 origin,
                                 unsigned int n_nodes,
                                 double minimal_node_width,
                                 SLTNode* root) :
    origin(origin),
    n_nodes(n_nodes),
    minimal_node_width(minimal_node_width),
    root(root) { 
}


SingleLightTree::~SingleLightTree() {
  delete this->root;
}


// --------------------------------------------------------------------------
//  Get | Set methods
// --------------------------------------------------------------------------
bool SingleLightTree::isInLight(glm::vec3 point) const {
  glm::vec3 orig = this->getOrigin();
  double width = this->getWidth();

  if ((point.x < orig.x)         ||
      (point.y < orig.y)         ||
      (point.z < orig.z)         ||
      (point.x > orig.x + width) ||
      (point.y > orig.y + width) ||
      (point.z > orig.z + width)) {
    return false;
  } else {
    return this->getRoot()->isInLight(point,
                                      NodeDimensions(orig, width));
  }
}

}
}
}
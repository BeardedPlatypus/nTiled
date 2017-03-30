#include "pipeline\light-management\hashed\light-octree\slt\nodes\SLTLeaf.h"


// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "pipeline\light-management\hashed\light-octree\slt\Exceptions.h"
#include "pipeline\light-management\hashed\light-octree\nodes\LOParent.h"
#include "pipeline\light-management\hashed\light-octree\nodes\LOLeaf.h"
#include "pipeline\light-management\hashed\light-octree\nodes\LOBranch.h"


namespace nTiled {
namespace pipeline {
namespace hashed {

SLTLeaf::SLTLeaf(bool has_light) : has_light(has_light) { 
}


SLTLeaf::SLTLeaf(NodeType node_type) : SLTLeaf(node_type == NodeType::Filled) {
  if (node_type == NodeType::Partial) throw SLTException();
}


bool SLTLeaf::isInLight(glm::vec3 point,
                       NodeDimensions dim) const {
  return this->hasLight();
}



void SLTLeaf::addToLeaf(LOLeaf* p_leaf, 
                        LOParent* p_parent,
                        glm::bvec3 node_index,
                        GLuint index) const {
  if (this->hasLight()) p_leaf->addIndex(index);
}


void SLTLeaf::addToBranch(LOBranch* p_branch, 
                          LOParent* p_parent,
                          glm::bvec3 node_index,
                          GLuint index) const {
  if (this->hasLight()) {
    for (unsigned int x = 0; x < 2; ++x) {
      for (unsigned int y = 0; y < 2; ++y) {
        for (unsigned int z = 0; z < 2; ++z) {
          glm::bvec3 child_index = glm::bvec3(x == 1,
                                              y == 1,
                                              z == 1);

          p_branch->getChildNode(child_index)->addSLTNode(*this, 
                                                          p_branch, 
                                                          child_index, 
                                                          index);
        }
      }
    }
  }
}


}
}
}
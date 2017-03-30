#include "pipeline\light-management\hashed\light-octree\slt\nodes\SLTBranch.h"

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\light-management\hashed\light-octree\slt\Exceptions.h"
#include "pipeline\light-management\hashed\light-octree\nodes\LOParent.h"
#include "pipeline\light-management\hashed\light-octree\nodes\LONode.h"
#include "pipeline\light-management\hashed\light-octree\nodes\LOLeaf.h"
#include "pipeline\light-management\hashed\light-octree\nodes\LOBranch.h"


namespace nTiled {
namespace pipeline {
namespace hashed {

SLTBranch::SLTBranch() { 
  for (unsigned int i = 0; i < 8; ++i) {
    has_set[i] = false;
  }
}


SLTBranch::~SLTBranch() { 
  for (unsigned int i = 0; i < 8; ++i) {
    if (has_set[i]) delete this->children[i];
  }
}


SLTNode* SLTBranch::getChildNode(glm::bvec3 position) const {
  unsigned int index = 0;

  if (position.x) index += 1;
  if (position.y) index += 2;
  if (position.z) index += 4;

  if (!(this->has_set[index])) throw SLTUnsetNodeAccess();

  return this->children[index];
}


void SLTBranch::setNode(glm::bvec3 position, SLTNode* new_node) {
  unsigned int index = 0;

  if (position.x) index += 1;
  if (position.y) index += 2;
  if (position.z) index += 4;

  if (this->has_set[index]) delete this->children[index];

  this->children[index] = new_node;
  this->has_set[index] = true;
}


bool SLTBranch::isInLight(glm::vec3 point,
                          NodeDimensions dim) const {
  glm::bvec3 index = dim.getNextIndex(point);
  return this->getChildNode(index)->isInLight(point, dim.getNextDimensions(index));
}


void SLTBranch::addToLeaf(LOLeaf* p_leaf, 
                          LOParent* p_parent,
                          glm::bvec3 node_index,
                          GLuint index) const {
  LONode* new_node = p_leaf->subdivide(p_parent, node_index);
  new_node->addSLTNode(*this, p_parent, node_index, index);
}


void SLTBranch::addToBranch(LOBranch* p_branch, 
                            LOParent* p_parent,
                            glm::bvec3 node_index,
                            GLuint index) const {
  for (unsigned int x = 0; x < 2; ++x) {
    for (unsigned int y = 0; y < 2; ++y) {
      for (unsigned int z = 0; z < 2; ++z) {
        glm::bvec3 child_index = glm::bvec3(x == 1,
                                            y == 1,
                                            z == 1);

        p_branch->getChildNode(child_index)->addSLTNode(
          *this->getChildNode(child_index),
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
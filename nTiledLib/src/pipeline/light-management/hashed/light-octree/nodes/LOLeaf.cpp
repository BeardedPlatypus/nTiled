#include "pipeline\light-management\hashed\light-octree\nodes\LOLeaf.h"


// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "pipeline\light-management\hashed\light-octree\nodes\LOParent.h"
#include "pipeline\light-management\hashed\light-octree\nodes\LOBranch.h"

namespace nTiled {
namespace pipeline {
namespace hashed {

// ----------------------------------------------------------------------------
//  Constructors
// ----------------------------------------------------------------------------
LOLeaf::LOLeaf() : 
    indices({}) {
}

LOLeaf::LOLeaf(const LOLeaf& node_ref) : 
    indices(std::vector<GLuint>(node_ref.getIndices())) {
}


// ----------------------------------------------------------------------------
//  Get methods
// ----------------------------------------------------------------------------
std::vector<GLuint> LOLeaf::retrieveLights(glm::vec3 point, 
                                           NodeDimensions node) const {
  return this->getIndices();
}

// ----------------------------------------------------------------------------
//  Construction methods
// ----------------------------------------------------------------------------
void LOLeaf::addSLTNode(const SLTNode& node, 
                        LOParent* p_parent,
                        glm::bvec3 node_index,
                        GLuint index) {
  node.addToLeaf(this, p_parent, node_index, index);
}


LONode* LOLeaf::retrieveAndConstructRoot(unsigned int depth_left,
                                         glm::vec3 mid_point_slt,
                                         NodeDimensions node_dim,
                                         LOParent* p_parent,
                                         glm::bvec3 index) {
  if (depth_left == 0) return this;

  LONode* new_node = this->subdivide(p_parent, index);
  return new_node->retrieveAndConstructRoot(depth_left,
                                            mid_point_slt,
                                            node_dim,
                                            p_parent,
                                            index);
}


LOBranch* LOLeaf::subdivide(LOParent* parent, glm::bvec3 index) {
  LOBranch* new_node = new LOBranch(this);

  parent->updateChild(index, new_node);
  return new_node;
}


void LOLeaf::addIndex(GLuint index) {
  this->indices.push_back(index);
}

}
}
}
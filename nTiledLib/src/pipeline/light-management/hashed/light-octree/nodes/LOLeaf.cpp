#include "pipeline\light-management\hashed\light-octree\nodes\LOLeaf.h"


// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "pipeline\light-management\hashed\light-octree\nodes\LOParent.h"
#include "pipeline\light-management\hashed\light-octree\nodes\LOBranch.h"
#include "pipeline\light-management\hashed\light-octree\nodes\LONodeContainer.h"


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


LONodeContainer LOLeaf::retrieveAndConstructRoot(unsigned int depth_left,
                                                 glm::vec3 mid_point_slt,
                                                 NodeDimensions node_dim,
                                                 LOParent* p_parent,
                                                 glm::bvec3 index) {
  if (depth_left == 0) return LONodeContainer(this, p_parent, index);

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


void LOLeaf::addToConstructionVectors(glm::uvec3 position,
                                        std::vector<std::pair<glm::uvec3, const LOBranch*>>& partials,
                                        std::vector<std::pair<glm::uvec3, const LOLeaf*>>& leaves) const {
  leaves.push_back(std::pair<glm::uvec3, const LOLeaf*>(position, this));
}


void LOLeaf::addIndex(GLuint index) {
  this->indices.push_back(index);
}


LOLeaf* LOLeaf::copy() const {
  return new LOLeaf(*this);
}


void LOLeaf::retrieveNodesAtDepth(unsigned int depth_left,
                                  glm::uvec3 position,
                                  std::vector<std::pair<glm::uvec3, const LOBranch*>>& nodes) const {
  if (depth_left == 0) {
    nodes.push_back(std::pair<glm::uvec3, LOBranch*>(position, new LOBranch(this->copy())));
  } else {
    glm::bvec3 child_index;
    for (unsigned int x = 0; x < 2; ++x) {
      for (unsigned int y = 0; y < 2; ++y) {
        for (unsigned int z = 0; z < 2; ++z) {
          this->retrieveNodesAtDepth(depth_left - 1,
                                     position + position + glm::uvec3(x, y, z),
                                     nodes);
        }
      }
    }
  }
}

}
}
}
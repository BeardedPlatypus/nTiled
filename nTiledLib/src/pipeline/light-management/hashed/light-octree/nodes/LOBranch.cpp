#include "pipeline\light-management\hashed\light-octree\nodes\LOBranch.h"


// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "pipeline\light-management\hashed\light-octree\nodes\LOLeaf.h"
#include "pipeline\light-management\hashed\light-octree\nodes\LONodeContainer.h"


namespace nTiled {
namespace pipeline {
namespace hashed {

// ----------------------------------------------------------------------------
//  Constructor | Destructor
// ----------------------------------------------------------------------------
LOBranch::LOBranch() {
  for (unsigned int i = 0; i < 8; ++i) {
    this->children[i] = new LOLeaf();
  }
}


LOBranch::LOBranch(LOLeaf* leaf) {
  this->children[0] = leaf;

  for (unsigned int i = 1; i < 8; ++i) {
    this->children[i] = new LOLeaf(*leaf);
  }
}


LOBranch::LOBranch(const LOBranch& node_ref) {
  for (unsigned int x = 0; x < 2; ++x) {
    for (unsigned int y = 0; y < 2; ++y) {
      for (unsigned int z = 0; z < 2; ++z) {
        const LONode& child = node_ref.getChildNodeConst(glm::bvec3(x == 1,
                                                                    y == 1,
                                                                    z == 1));
        this->children[x + y * 2 + z * 4] = child.copy();
      }
    }
  }
}


LOBranch::~LOBranch() {
  for (unsigned int i = 0; i < 8; ++i) {
    delete this->children[i];
  }
}

// ----------------------------------------------------------------------------
//  Get methods
// ----------------------------------------------------------------------------
LONode* LOBranch::getChildNode(glm::bvec3 position) {
  unsigned int index = 0;

  if (position.x) index += 1;
  if (position.y) index += 2;
  if (position.z) index += 4;

  return this->children[index];
}


const LONode& LOBranch::getChildNodeConst(glm::bvec3 position) const {
  unsigned int index = 0;

  if (position.x) index += 1;
  if (position.y) index += 2;
  if (position.z) index += 4;

  return *(this->children[index]);
}

// ----------------------------------------------------------------------------
//  LONode methods
// ----------------------------------------------------------------------------
std::vector<GLuint> LOBranch::retrieveLights(glm::vec3 point,
                                             NodeDimensions node) const {
  glm::bvec3 index = node.getNextIndex(point);
  return this->getChildNodeConst(index).retrieveLights(point,
                                                       node.getNextDimensions(index));
}


void LOBranch::addSLTNode(const SLTNode& node, 
                          LOParent* p_parent,
                          glm::bvec3 node_index,
                          GLuint index) {
  node.addToBranch(this, p_parent, node_index, index);
}


LONodeContainer LOBranch::retrieveAndConstructRoot(unsigned int depth_left,
                                                   glm::vec3 mid_point_slt,
                                                   NodeDimensions node_dim,
                                                   LOParent* p_parent,
                                                   glm::bvec3 index) {
  if (depth_left == 0) return LONodeContainer(this, p_parent, index);

  glm::bvec3 next_index = node_dim.getNextIndex(mid_point_slt);
  return this->getChildNode(next_index)->retrieveAndConstructRoot(depth_left - 1,
                                                                  mid_point_slt,
                                                                  node_dim.getNextDimensions(next_index),
                                                                  this,
                                                                  next_index);
}



void LOBranch::addToConstructionVectors(glm::uvec3 position,
                                        std::vector<std::pair<glm::uvec3, const LOBranch*>>& partials,
                                        std::vector<std::pair<glm::uvec3, const LOLeaf*>>& leaves) const {
  partials.push_back(std::pair<glm::uvec3, const LOBranch*>(position, this));
}


void LOBranch::updateChild(glm::bvec3 index, LONode* child) {
  unsigned int i = 0;

  if (index.x) i += 1;
  if (index.y) i += 2;
  if (index.z) i += 4;

  this->children[i] = child;
}


LOBranch* LOBranch::copy() const {
  return new LOBranch(*this);
}


void LOBranch::retrieveNodesAtDepth(unsigned int depth_left,
                                    glm::uvec3 position,
                                    std::vector<std::pair<glm::uvec3, const LOBranch*>>& nodes) const {
  if (depth_left == 0) {
    nodes.push_back(std::pair<glm::uvec3, LOBranch*>(position, this->copy()));
  } else {
    glm::bvec3 child_index;
    for (unsigned int x = 0; x < 2; ++x) {
      for (unsigned int y = 0; y < 2; ++y) {
        for (unsigned int z = 0; z < 2; ++z) {
          child_index = glm::bvec3(x == 1,
                                   y == 1,
                                   z == 1);

          this->getChildNodeConst(child_index).retrieveNodesAtDepth(depth_left - 1,
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
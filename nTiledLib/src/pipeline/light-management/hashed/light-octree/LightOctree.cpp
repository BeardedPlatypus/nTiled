#include "pipeline\light-management\hashed\light-octree\LightOctree.h"


// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "pipeline\light-management\hashed\light-octree\nodes\LOLeaf.h"

namespace nTiled {
namespace pipeline {
namespace hashed {


// ----------------------------------------------------------------------------
//  Constructor | Destructor
// ----------------------------------------------------------------------------
LightOctree::LightOctree(glm::vec3 origin,
                         unsigned int depth,
                         double minimal_node_size) :
    origin(origin),
    depth(depth),
    minimal_node_size(minimal_node_size),
    root(new LOLeaf()) {
}


LightOctree::~LightOctree() {
  delete this->root;
}


// ----------------------------------------------------------------------------
//  Get Methods
// ----------------------------------------------------------------------------
std::vector<GLuint> LightOctree::retrieveLights(glm::vec3 point) const {
  // check whether point falls within light octree, if not return empty
  glm::vec3 orig = this->getOrigin();
  double width = this->getWidth();
  if (point.x < orig.x ||
      point.y < orig.y ||
      point.z < orig.z ||
      point.x > orig.x + width ||
      point.y > orig.y + width ||
      point.z > orig.z + width) {
    return std::vector<GLuint>();
  }

  return this->root->retrieveLights(point, 
                                    NodeDimensions(this->getOrigin(),
                                                   this->getWidth()));
}


// ----------------------------------------------------------------------------
//  LinklessOctree construction related methods
// ----------------------------------------------------------------------------
std::vector<std::pair<glm::uvec3, const LOBranch*>> LightOctree::retrieveNodesAtDepth(unsigned int depth_i) const {
  std::vector<std::pair<glm::uvec3, const LOBranch*>> nodes = {};

  this->getRootConst().retrieveNodesAtDepth(depth_i,
                                            glm::uvec3(0),
                                            nodes);

  return nodes;
}


// ----------------------------------------------------------------------------
//  Construction Methods
// ----------------------------------------------------------------------------
void LightOctree::addSLT(const SingleLightTree& slt, GLuint index) {
  LONodeContainer slt_root_node = this->constructAndRetrieveRoot(slt);
  slt_root_node.p_node->addSLTNode(*(slt.getRoot()), 
                                   slt_root_node.p_parent,
                                   glm::bvec3(false),
                                   index);
}


LONodeContainer LightOctree::constructAndRetrieveRoot(const SingleLightTree& slt) {
  unsigned int depth_left = this->getDepth() - slt.getDepth();
  glm::vec3 mid_point_slt = slt.getOrigin() + glm::vec3(0.5 * slt.getWidth());
  NodeDimensions node_dim = NodeDimensions(this->getOrigin(), this->getWidth());

  return this->getRoot()->retrieveAndConstructRoot(depth_left,
                                                   mid_point_slt,
                                                   node_dim,
                                                   this,
                                                   glm::bvec3(false));
}


void LightOctree::updateChild(glm::bvec3 _, LONode* child) {
  this->root = child;
}




} // hashed
} // pipeline
} // nTiled

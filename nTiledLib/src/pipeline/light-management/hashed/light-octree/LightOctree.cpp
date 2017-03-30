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
//  Construction Methods
// ----------------------------------------------------------------------------
void LightOctree::addSLT(const SingleLightTree& slt, GLuint index) {
  LONode* slt_root_node = this->constructAndRetrieveRoot(slt);
  slt_root_node->addSLTNode(*(slt.getRoot()), 
                            this,
                            glm::bvec3(false),
                            index);
}


LONode* LightOctree::constructAndRetrieveRoot(const SingleLightTree& slt) {
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

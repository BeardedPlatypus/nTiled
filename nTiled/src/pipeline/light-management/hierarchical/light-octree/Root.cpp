#include "pipeline\light-management\hierarchical\light-octree\Root.h"

#include "pipeline\light-management\hierarchical\light-octree\Leaf.h"

namespace nTiled {
namespace pipeline {
namespace hierarchical {
namespace lo {

Root::Root() : Node(nullptr, 0) {
  this->p_root = new Leaf(this, 0);
}


Root::~Root() {
  delete this->p_root;
}


void Root::addSLT(const SingleLightTree& slt, 
                  unsigned int depth, 
                  glm::uvec3 current_middle) { 
  this->p_root->addSLT(slt, depth, current_middle);
}


void Root::addSLTNode(const SingleLightTree& slt,
                      const slt::Node& node) {
  this->p_root->addSLTNode(slt, node);
}


void Root::addLight(GLuint light_index) {
  this->p_root->addLight(light_index);
}


void Root::addPartialNode(const SingleLightTree& slt, 
                          const slt::PartialLightNode& partial_node) {
  this->p_root->addPartialNode(slt, partial_node);
}


 bool Root::isEmpty() const{ 
   return this->p_root->isEmpty(); 
}


void Root::exportToJson(rapidjson::Writer<rapidjson::StringBuffer>& writer) const {
    this->p_root->exportToJson(writer);
}


void Root::addSLTInternal(const SingleLightTree& slt, 
                          unsigned int depth, 
                          glm::uvec3 current_middle) {
  throw UnsupportedMethodException();
}


void Root::updateChild(Node* child) {
  this->p_root = child;
}

} // lo
} // hierarchical
} // pipeline
} // nTiled

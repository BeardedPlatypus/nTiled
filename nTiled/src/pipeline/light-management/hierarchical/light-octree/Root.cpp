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


void Root::extractData(glm::uvec3 point,
                       std::vector<std::pair<glm::uvec3, glm::u8vec2>>& hash_nodes,
                       std::vector<std::pair<glm::uvec3, glm::uvec2>>& leaf_nodes,
                       std::vector<GLuint>& light_index_list,
                       std::vector<std::pair<glm::uvec3, Node*>> next_nodes) const {
  this->p_root->extractData(point, hash_nodes, leaf_nodes, light_index_list, next_nodes);
}


void Root::extractDataLocal(glm::uvec3 point,
                            std::vector<std::pair<glm::uvec3, glm::uvec2>>& leaf_nodes,
                            std::vector<GLuint>& light_index_list,
                            std::vector<std::pair<glm::uvec3, Node*>>& next_nodes) {
  this->p_root->extractDataLocal(point, leaf_nodes, light_index_list, next_nodes);
}

void Root::getSubNodes(glm::uvec3 current_point, std::vector<std::pair<glm::uvec3, Node*>>& node_list) {
  return this->p_root->getSubNodes(current_point, node_list);
}

} // lo
} // hierarchical
} // pipeline
} // nTiled

#include "pipeline\light-management\hierarchical\light-octree\Leaf.h"

// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "pipeline\light-management\hierarchical\light-octree\Branch.h"

namespace nTiled {
namespace pipeline {
namespace hierarchical {
namespace lo {

// ----------------------------------------------------------------------------
//  Constructor
// ----------------------------------------------------------------------------
Leaf::Leaf(Parent* p_parent, unsigned char index) : 
  Node(p_parent, index), 
  light_index_list(std::vector<GLuint>()) { }


Leaf::Leaf(Parent* p_parent, 
           unsigned char index, 
           const std::vector<GLuint>& light_index_list) : 
  Node(p_parent, index),
  light_index_list(std::vector<GLuint>(light_index_list)) { }


void Leaf::addSLTInternal(const SingleLightTree& slt,
                          unsigned int depth,
                          glm::uvec3 current_middle) {
  Branch* node = this->replaceWithBranch();
  node->addSLT(slt, depth, current_middle);
}


void Leaf::addSLTNode(const SingleLightTree& slt,
                      const slt::Node& node) {
  node.addToOctreeNode(this, slt);
}


void Leaf::addLight(GLuint light_index) {
  this->light_index_list.push_back(light_index);
}


void Leaf::addPartialNode(const SingleLightTree& slt,
                          const slt::PartialLightNode& partial_node) {
  Branch* node = this->replaceWithBranch();
  node->addPartialNode(slt, partial_node);
}


Branch* Leaf::replaceWithBranch() {
  Branch* new_node = new Branch(this->p_parent, 
                                this->getIndex(), 
                                this);
  return new_node;
}


void Leaf::exportToJson(rapidjson::Writer<rapidjson::StringBuffer>& writer) const {
  writer.StartObject();
    writer.Key("type");
    writer.String("leaf");

    writer.Key("light-indices");
    writer.StartArray();
    
    for (GLuint light_index : this->light_index_list ) {
      writer.Uint((unsigned int)(light_index));
    }

    writer.EndArray();
  writer.EndObject();
}


glm::bvec2 Leaf::toHashNode() const {
  return glm::bvec2(true, !this->isEmpty());
}


void Leaf::extractData(glm::uvec3 point,
                       std::vector<std::pair<glm::uvec3, glm::bvec2>>& hash_nodes,
                       std::vector<std::pair<glm::uvec3, glm::uvec2>>& leaf_nodes,
                       std::vector<GLuint>& light_index_list,
                       std::vector<std::pair<glm::uvec3, Node*>> next_nodes) const {
  // add this to hash node
  hash_nodes.push_back(std::pair<glm::uvec3, glm::bvec2>(point, this->toHashNode()));
  
  // add leaf specific
  std::vector<GLuint> indices = this->getLightIndices();
  leaf_nodes.push_back(std::pair<glm::uvec3, glm::uvec2>(point, glm::uvec2(light_index_list.size(), indices.size())));
  light_index_list.insert(light_index_list.end(), indices.begin(), indices.end());
}


void Leaf::getSubNodes(glm::uvec3 current_point, std::vector<std::pair<glm::uvec3, Node*>>& node_list) {
  // dirty method
  glm::uvec3 next_origin = glm::uvec3(current_point.x * 2, current_point.y * 2, current_point.z * 2);
  node_list.push_back(std::pair<glm::uvec3, Node*>(next_origin + glm::uvec3(0, 0, 0), this));
  node_list.push_back(std::pair<glm::uvec3, Node*>(next_origin + glm::uvec3(1, 0, 0), this));
  node_list.push_back(std::pair<glm::uvec3, Node*>(next_origin + glm::uvec3(0, 1, 0), this));
  node_list.push_back(std::pair<glm::uvec3, Node*>(next_origin + glm::uvec3(1, 1, 0), this));
  node_list.push_back(std::pair<glm::uvec3, Node*>(next_origin + glm::uvec3(0, 0, 1), this));
  node_list.push_back(std::pair<glm::uvec3, Node*>(next_origin + glm::uvec3(1, 0, 1), this));
  node_list.push_back(std::pair<glm::uvec3, Node*>(next_origin + glm::uvec3(0, 1, 1), this));
  node_list.push_back(std::pair<glm::uvec3, Node*>(next_origin + glm::uvec3(1, 1, 1), this));
}

}
}
}
}
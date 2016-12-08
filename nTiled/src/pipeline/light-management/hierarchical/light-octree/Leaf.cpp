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
  Branch* new_node = new Branch(this->p_parent, this->getIndex(), this);
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


}
}
}
}
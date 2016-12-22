#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glm\glm.hpp>
#include <glad\glad.h>

// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "pipeline\light-management\hierarchical\single-light-tree\SingleLightTree.h"

namespace nTiled {
namespace pipeline {
namespace hierarchical {
namespace lo {

class Parent;

class Node {
public:
  Node(Parent* p_parent, unsigned char index);
  //virtual ~Node() = 0;
  
  virtual void addSLT(const SingleLightTree& slt, 
                      unsigned int depth, 
                      glm::uvec3 current_middle);
  virtual void addSLTNode(const SingleLightTree& slt,
                          const slt::Node& node) = 0;
  virtual void addLight(GLuint light_index) = 0;
  virtual void addPartialNode(const SingleLightTree& slt,
                              const slt::PartialLightNode& partial_node) = 0;

  virtual bool isEmpty() const = 0;

  void setParent(Parent* new_parent) { p_parent = new_parent; }

  unsigned char getIndex() const { return this->index; }

  virtual glm::bvec2 toHashNode() const = 0;

  virtual void extractData(glm::uvec3 point,
                           std::vector<std::pair<glm::uvec3, glm::bvec2>>& hash_nodes,
                           std::vector<std::pair<glm::uvec3, glm::uvec2>>& leaf_nodes,
                           std::vector<GLuint>& light_index_list,
                           std::vector<std::pair<glm::uvec3, Node*>> next_nodes) const = 0;

  virtual void getSubNodes(glm::uvec3 current_point, std::vector<std::pair<glm::uvec3, Node*>>& node_list) = 0;

  virtual void exportToJson(rapidjson::Writer<rapidjson::StringBuffer>& writer) const = 0;
protected:
  virtual void addSLTInternal(const SingleLightTree& slt,
                              unsigned int depth,
                              glm::uvec3 current_middle) = 0;
protected:
  Parent* p_parent;
private:
  unsigned char index;
};

}
}
}
}
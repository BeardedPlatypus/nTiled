#pragma once
// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glm\glm.hpp>
#include <glad\glad.h>

// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "pipeline\light-management\hashed\light-octree\single-light-tree\SingleLightTree.h"

namespace nTiled {
namespace pipeline {
namespace hashed {
namespace lo {

class Parent;

class Node {
public:
  Node(Parent* p_parent, unsigned char index);

  virtual void addSLT(unsigned int current_depth,
                      glm::uvec3 current_middle,
                      const SingleLightTree& slt);

  virtual void addSLTNode(const SingleLightTree& slt,
                          const slt::Node& node) = 0;

  virtual void addLight(GLuint light_index) = 0;

  virtual void addPartialNode(const SingleLightTree& slt,
                              const slt::Partial& partial_node) = 0;

  virtual bool isEmpty() const = 0;

  void setParent(Parent* new_parent) { p_parent = new_parent; }

  unsigned char getIndex() const { return this->index; }

  virtual glm::uvec2 toHashNode() const = 0;

  virtual void extractData(glm::uvec3 point,
                           std::vector<std::pair<glm::uvec3, glm::u8vec2>>& hash_nodes,
                           std::vector<std::pair<glm::uvec3, glm::uvec2>>& leaf_nodes,
                           std::vector<GLuint>& light_index_list,
                           std::vector<std::pair<glm::uvec3, Node*>>& next_nodes) const = 0;

  virtual void extractDataLocal(glm::uvec3 point,
                                std::vector<std::pair<glm::uvec3, glm::uvec2>>& leaf_nodes,
                                std::vector<GLuint>& light_index_list,
                                std::vector<std::pair<glm::uvec3, Node*>>& next_nodes) = 0;

  virtual void getSubNodes(glm::uvec3 current_point, std::vector<std::pair<glm::uvec3, Node*>>& node_list) = 0;

  virtual void exportToJson(rapidjson::Writer<rapidjson::StringBuffer>& writer) const = 0;


protected:
  virtual void addSLTInternal(unsigned int depth,
                              glm::uvec3 current_middle,                              
                              const SingleLightTree& slt) = 0;


  Parent* p_parent;
private:
  unsigned char index;
};

}
}
}
}

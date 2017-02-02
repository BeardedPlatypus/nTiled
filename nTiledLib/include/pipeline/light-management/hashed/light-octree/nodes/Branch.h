#pragma once
#include "Node.h"
#include "Parent.h"

namespace nTiled {
namespace pipeline {
namespace hashed {
namespace lo {

class Leaf;

/*! @brief The Branch node specifies a branch node within the light octree
*         It is always considered non empty, and provides 8 subnodes of class
*         Node.
*/
class Branch : public Node, public Parent {
public:
  /*! @brief Construct a new Branch node with the given pointer to a parent Branch and
  *         subnodes equal to the given leaf node
  *         This also changes the leaf node.
  *
  * @param p_parent Pointer to the Branch node that is the parent of this new Branch
  * @param leaf_node The new subnodes of this Branch node.
  */
  Branch(Parent* p_parent, unsigned char index, Leaf* leaf_node);

  /*! @brief Construct a new Branch node with the given pointer to a parent Branch and
  *         empty Leaf nodes.
  *
  * @param p_parent Pointer to the Branch node that is the parent of this new Branch
  */
  Branch(Parent* p_parent, unsigned char index);

  virtual ~Branch();

  virtual void addSLTNode(const SingleLightTree& slt,
                          const slt::Node& node) override;
  virtual void addLight(GLuint light_index) override;

  /*! @brief Add the specified partial node to this Branch node.
  */
  virtual void addPartialNode(const SingleLightTree& slt,
                              const slt::Partial& partial_node);

  virtual bool isEmpty() const override { return false; }

  /*! @brief Set subnode of index child->getIndex() to child
  */
  virtual void updateChild(Node* child) override;

  virtual glm::uvec2 toHashNode() const;

  virtual void extractData(glm::uvec3 point,
                           std::vector<std::pair<glm::uvec3, glm::u8vec2>>& hash_nodes,
                           std::vector<std::pair<glm::uvec3, glm::uvec2>>& leaf_nodes,
                           std::vector<GLuint>& light_index_list,
                           std::vector<std::pair<glm::uvec3, Node*>>& next_nodes) const override;

  virtual void extractDataLocal(glm::uvec3 point,
                                std::vector<std::pair<glm::uvec3, glm::uvec2>>& leaf_nodes,
                                std::vector<GLuint>& light_index_list,
                                std::vector<std::pair<glm::uvec3, Node*>>& next_nodes) override;

  virtual void getSubNodes(glm::uvec3 current_point, std::vector<std::pair<glm::uvec3, Node*>>& node_list);

  virtual void exportToJson(rapidjson::Writer<rapidjson::StringBuffer>& writer) const override;
protected:
  virtual void addSLTInternal(unsigned int depth, 
                              glm::uvec3 current_middle,
                              const SingleLightTree& slt) override;

private:
  /*! @brief Get the pointer to the child node at index i
  *
  * @param i The index of the child Node pointer to be retrieved
  *
  * @return The pointer to the child Node at index i
  */
  Node* getChild(int i);

  /*! @brief Get the index of the child node specified by key
  *
  * @param key The key corresponding with boolean values for each axis
  *             representing the child node.
  *
  * @return The index to the child Node corresponding with the key
  */
  int getIndexChild(glm::bvec3 key) const;

  /*! @brief The sub nodes of this BranchNode. */
  Node* sub_nodes[8];
};

}
}
}
}
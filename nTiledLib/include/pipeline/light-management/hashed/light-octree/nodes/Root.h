#pragma once

// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "Node.h"
#include "Parent.h"


namespace nTiled {
namespace pipeline {
namespace hashed {
namespace lo {

/*! @brief The RootBranch contains the whole LightOctree and is a special
*         Branch which does not have a parent (the root)
*/
class Root : public Node, public Parent {
public:
  // --------------------------------------------------------------------------
  //  Constructor | Destructor
  // --------------------------------------------------------------------------
  /*! @brief Construct a new RootBranch with the given pointer to the root Node
  *
  * @param root The root Nod
  */
  Root();

  virtual ~Root();


  // --------------------------------------------------------------------------
  //  LightOctree methods
  // --------------------------------------------------------------------------
  virtual void addSLT(unsigned int depth,
                      glm::uvec3 current_middle,
                      const SingleLightTree& slt) override;

  virtual void addSLTNode(const SingleLightTree& slt,
                          const slt::Node& node);

  virtual void addLight(GLuint light_index) override;

  virtual void addPartialNode(const SingleLightTree& slt,
                              const slt::Partial& partial_node) override;

  virtual bool isEmpty() const override;

  virtual void updateChild(Node* child) override;

  virtual void exportToJson(rapidjson::Writer<rapidjson::StringBuffer>& writer) const override;

  virtual glm::uvec2 toHashNode() const { return glm::uvec2(0, 0); }

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

protected:
  virtual void addSLTInternal(unsigned int depth, 
                              glm::uvec3 current_middle,
                              const SingleLightTree& slt) override;

private:
  /*! @brief The root node of this LightOctree. */
  Node* p_root;
};

}
}
}
}

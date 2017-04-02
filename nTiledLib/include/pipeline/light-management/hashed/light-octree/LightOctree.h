#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <vector>
#include <glm\glm.hpp>
#include <glad\glad.h>

// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "math\octree.h"
#include "nodes\LONode.h"
#include "nodes\LOParent.h"
#include "nodes\LONodeContainer.h"

#include "slt\SingleLightTree.h"


namespace nTiled {
namespace pipeline {
namespace hashed {

class LightOctree : public LOParent {
public:
  // --------------------------------------------------------------------------
  //  Constructor | Destructor 
  // --------------------------------------------------------------------------
  /*! @brief Construct a new empty LightOctree with the given origin, depth and
   *         node_size
   * 
   * @param origin The origin of this new empty LightOctree
   * @param depth The depth of this new empty LightOctree
   * @param minimal_node_size The minimal node size of this new empty 
   *                          LightOctree
   */
  LightOctree(glm::vec3 origin,
              unsigned int depth,
              double minimal_node_size);

  /*! @brief Destruct this LightOctree. 
   */
  ~LightOctree();

  // --------------------------------------------------------------------------
  //  Get Methods
  // --------------------------------------------------------------------------
  /*! @brief Get the root of this LightOctree. 
   * 
   * @returns The root of this LightOctree
   */
  LONode* getRoot() { return this->root; }

  const LONode& getRootConst() const { return *this->root; }

  /*! @brief Get the origin of this LightOctree.
   *
   * @returns The origin of this LightOctree
   */
  glm::vec3 getOrigin() const { return this->origin; }

  /*! @brief Get the depth of this LightOctree. 
   * 
   * @returns The depth of this LightOctree
   */
  unsigned int getDepth() const { return this->depth; }

  /*! @brief Get the minimal node size of this LightOctree
   *
   * @returns The minimal node size of this LightOctree.
   */
  double getMinimalNodeSize() const { return this->minimal_node_size; }

  /*! @brief Get the number of minimum nodes in a single dimension
   *         of this LightOctree.
   * 
   * @returns The number of minimal nodes in a single dimension of 
   *          of this LightOctree
   */
  unsigned int getNNodes() const { return math::calculateNNodes(this->getDepth() - 1); }

  /*! @brief Get the width of this LightOctree. 
   * 
   * @returns The width of this LightOctree
   */
  double getWidth() const { return this->getNNodes() * this->getMinimalNodeSize(); }

  // --------------------------------------------------------------------------
  /*! @brief Retrieve the light indices associated with the provided point
   * 
   * @param point The point of which the light indices should be retrieved
   *
   * @returns Indices of all lights that potentially effect this point.
   */
  std::vector<GLuint> retrieveLights(glm::vec3 point) const;

  // --------------------------------------------------------------------------
  //  LinklessOctree construction related methods
  // --------------------------------------------------------------------------
  /*! @brief Retrieve and copy all the nodes at depth i
   *
   * param depth_i The depth of which the nodes should be retrieved.
   *
   * @returns A vector containing all nodes and corresponding locations at
   *          depth i
   */
  std::vector<std::pair<glm::uvec3, const LOBranch*>> retrieveNodesAtDepth(unsigned int depth_i) const;


  // --------------------------------------------------------------------------
  //  Construction methods
  // --------------------------------------------------------------------------
  /*! @brief Add the specified SingleLightTree to this LightOctree
   * 
   * @param slt The SingleLightTree which should be added to this light
   */
  void addSLT(const SingleLightTree& slt, GLuint index);

  /*! @brief Retrieve the node that corresponds with the root of the 
   *         SingleLightTree, in case such a node does not exist, it is 
   *         created.
   *
   * @param slt constant reference to the SingleLightTree of which the root 
   *            node within this LightOctree should be retrieved.
   * @param index The index associated with the SingleLightTree.
   *
   * @returns The node within this LightOctree that corresponds with the root
   *          node of the SingleLightTree
   */
  LONodeContainer constructAndRetrieveRoot(const SingleLightTree& slt);

  virtual void updateChild(glm::bvec3 index, LONode* child);

private:
  /*! @brief The origin of this LightOctree. */
  glm::vec3 origin;

  /*! @brief The depth of this LightOctree. */
  unsigned int depth;

  /*! @brief The minimal node size of this LightOctree. */
  const double minimal_node_size;

  /*! @brief The root of this LightOctree. */
  LONode* root;
};

}
}
}
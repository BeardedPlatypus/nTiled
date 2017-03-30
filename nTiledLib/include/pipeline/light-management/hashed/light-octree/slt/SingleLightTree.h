#pragma once


// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glm\glm.hpp>

// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "nodes\SLTNode.h"

namespace nTiled {
namespace pipeline {
namespace hashed {

class SingleLightTree {
public:
  // --------------------------------------------------------------------------
  //  Constructor | Destructor
  // --------------------------------------------------------------------------
  /*! @brief Construct a new SingleLightTree with the provided origin, width 
   *         and rootnode
   * 
   * @param origin The origin of this new SingleLightTree
   * @param n_nodes The number of nodes in a single dimension within this new 
   *                           SingleLightTree
   * @param minimal_node_width The width of the smallest node within this new
   *                           SingleLightTree
   * @param root The root node of this new SingleLightTree
   */
  SingleLightTree(glm::vec3 origin,
                  unsigned int n_nodes,
                  double minimal_node_width,
                  SLTNode* root);

  /*! @brief Destruct this SingleLightTree. 
   */
  ~SingleLightTree();

  // --------------------------------------------------------------------------
  //  Get methods
  // --------------------------------------------------------------------------
  /*! @brief Get the origin of this SingleLightTree.
   *
   * @returns The origin of this SingleLightTree
   */
  glm::vec3 getOrigin() const { return this->origin; }

  /*! @brief Get the width of this SingleLightTree
   *         this is equal to the size of the root node.
   *
   * @returns the width of this SingleLightTree
   */
  double getWidth() const { return this->getNNodes() * this->getMinimalNodeSize(); }

  /*! @brief Get the number of nodes in a single dimension within this 
   *         SingleLightTree
   *
   * @returns The number of nodes in a single dimension
   */
  unsigned int getNNodes() const { return this->n_nodes; }

  /*! @brief Get the depth of this SingleLightOctree 
   * 
   * @returns the depth of this SingleLightOctree
   */
  unsigned int getDepth() const { return unsigned int(log2(this->getNNodes())) + 1; }

  /*! @brief Get the size of the smallest node within this SingleLightTree
   *
   * @returns the size of minimal node
   */
  double getMinimalNodeSize() const { return this->minimal_node_width; }

  /*! @brief Get the root of this SingleLightTree
   *
   * @returns The root of this SingleLightTree
   */
  SLTNode* getRoot() const { return this->root; }

  // --------------------------------------------------------------------------
  //  queryMethods
  // --------------------------------------------------------------------------
  /*! @brief Query this SingleLightTree to see whether the point is contained
   *         in a cell that contains a light.
   * 
   * param point The point being queried
   * 
   * return true if the cell of the provided point contains a light, false 
   *        otherwise
   */
  bool isInLight(glm::vec3 point) const;

private:
  /*! @brief The root of this SingleLightTree. */
  SLTNode* root;

  /*! @brief The origin of this SingleLightTree. */
  glm::vec3 origin;

  /*! @brief The size of the smallest node within this SingleLightTree. */
  double minimal_node_width;

  /*! @brief The number of smallest nodes in a single dimension within this 
   *         SingleLightTree*/
  unsigned int n_nodes;

};

} // hashed
} // pipeline
} // nTiled
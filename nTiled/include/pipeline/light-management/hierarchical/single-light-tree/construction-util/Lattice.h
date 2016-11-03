/*! @file SLTLattice
 *  @brief SLTLattice contains the definition of the SingleLightTree lattice
 *         used in the construction of SingleLightTree
 */
#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glm\glm.hpp>
#include <vector>


// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "LatticeNode.h"


namespace nTiled {
namespace pipeline {
namespace hierarchical {
namespace slt {


/*! @brief slt::Lattice describes a 3d grid containing
 *         slt::LatticeNodes. A slt::Lattice describes a single layer
 *         in the construction of a SingleLightTree.
 *         It provides the methods to initialise it with both slt::FullLight
 *         or slt::NoLight nodes.
 *         The lattice coordinate system is similar to world, however it
 *         has been clamped to multiples of the NodeSize
 */
class Lattice {
public:
  // --------------------------------------------------------------------------
  //  Constructors || Destructor
  // --------------------------------------------------------------------------

  /*! @brief Construct a new slt::Lattice with the given parameters
   *         containing an array of size n_nodes^3
   *
   * @param origin_in_lattice The origin in regards to lattice coordinates of
   *                          this new SLTLattice.
   * @param n_nodes The number of nodes in each dimension of this new
   *                 SLTLattice
   * @param node_size The width of a single node in this new SLTLattice
   *
   */
  Lattice(const glm::ivec3 origin_in_lattice,
             const unsigned int n_nodes,
             const float node_size);

  /*! @brief Construct a new slt::Lattice with the given parameters containing 
   *         a vector of size n_nodes^3 initialised with p_node.
   *
   * @param origin_in_lattice The origin in regards to lattice coordinates of
   *                          this new SLTLattice.
   * @param n_nodes The number of nodes in each dimension of this new
   *                 SLTLattice
   * @param node_size The width of a single node in this new SLTLattice
   * @param p_node pointer to the lattice node with which this lattice 
   *               will be filled
   */
  Lattice(const glm::ivec3 origin_in_lattice,
          const unsigned int n_nodes,
          const float node_size,
          LatticeNode* p_node);

  /*! @brief Construct a new slt::Lattice initialised with NoLight nodes
   */
  Lattice(const glm::ivec3 origin_in_lattice,
             const unsigned int n_nodes,
             const float node_size,
             const NoLightNode& no_light);

  /*! @brief Construct a new Lattice initialised with FullLight nodes
   */
  Lattice(const glm::ivec3 origin_in_lattice,
             const unsigned int n_nodes,
             const float node_size,
             const FullLightNode& full_light);

  // --------------------------------------------------------------------------
  /*! @brief Deconstruct this SLTLattice
   */
  ~Lattice();

  // --------------------------------------------------------------------------
  /*! @brief Get the SLTLatticeNode at the given position
   *
   * @param position The position at which the returned SLTLatticeNode resides
   *
   * @return The SLTLatticeNode at position
   */
  const LatticeNode& getLatticeNode(glm::uvec3 position) const;

  /*! @brief Set the SLTLatticeNode at the given position to node.
   *
   * @param position The position at which the node should be placed
   * @param node The node to which the node at position should be set
   *
   * @post this->getLatticeNode(position) == node
   */
  void setLatticeNode(glm::uvec3 position,
                      const LatticeNode& node);

  /*! @brief Set the SLTLatticeNode at the given position to a NoLight node
   *
   * @param position The position at which the node should be placed
   * @param no_light The node to which the SLTLatticeNode should refer
   *
   * @post this->getLatticeNode(position) == SLTLatticeNode(NoLight, no_light)
   */
  void setLatticeNode(glm::uvec3 position,
                      const NoLightNode& no_light);

  /*! @brief Set the SLTLatticeNode at the given position to a FullLight node
   *
   * @param position The position at which the node should be placed
   * @param full_light The node to which the SLTLatticeNode should refer
   *
   * @post this->getLatticeNode(position) ==
   *        SLTLatticeNode(FullLight, full_light)
   */
  void setLatticeNode(glm::uvec3 position,
                      const FullLightNode& full_light);


  /*! @brief Set the SLTLatticeNode at the given position to a PartialLight
   *        node
   *
   * @param position The position at which the node should be placed
   * @param no_light The node to which the SLTLatticeNode should refer
   *
   * @post this->getLatticeNode(position) == S
   *       LTLatticeNode(PartialLight, partial_light)
   */
  void setLatticeNode(glm::uvec3 position,
                      const PartialLightNode& partial_light);

  /*! @brief Get the number of nodes in each dimension of this SLTLattice
   *
   * @return The number of nodes in each dimension of this SLTLattice.
   */
  const unsigned int getNNodes() const { return this->n_nodes; }

  /*! @brief Get the origin of this Lattice in respect to Lattice Coordinates
   *
   * @return The origin of this SLTLattice in respect to Lattice Coordinates
   */
  glm::ivec3 getOriginInLattice() const { return this->origin_in_grid; }

  /*! @brief Get the origin of this Lattice in respect ot World Coordinates
   *
   * @return The origin of this SLTLattice in respect to World Coordinates.
   */
  glm::vec4 getOriginInWorld() const {
    return (glm::vec4((glm::vec3(this->origin_in_grid) * node_size), 1.0));
  }

  const float getNodeSize() const { return this->node_size; }

private:
  /*! @brief Set the internal lattice node pointer to the provided pointer
   *
   * @param position position of the node pointer that should be updated
   * @param p_node pointer to the new node that should be placed at the position
   */
  void setLatticeNode(glm::uvec3 position,
                      LatticeNode* p_node);

  /*! @brief Get the internal index of the node at position
   *
   * @return the internal index of the node at position
   */
  int getIndexNode(glm::uvec3 position) const;

  /*! @brief The position of this SLTLattice in Lattice Coordinates */
  const glm::ivec3 origin_in_grid;

  /*! @brief The number of nodes in each dimension of this SLTLattice */
  const unsigned int n_nodes;

  /*! @brief The size of a single node in a single dimension of this SLTLattice */
  const float node_size;

  /*! @brief An array of SLTLatticeNode pointers, representing
   *         the internal lattice
   */
  std::vector<LatticeNode*> lattice_nodes;
  std::vector<LatticeNode*> unique_nodes;
};

} // slt
} // hierarchical
} // pipeline
} // nTiled

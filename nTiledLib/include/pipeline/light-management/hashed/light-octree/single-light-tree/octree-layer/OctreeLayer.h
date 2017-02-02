#pragma once
// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glm\glm.hpp>
#include <vector>


// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "OctreeLayerNode.h"


namespace nTiled {
namespace pipeline {
namespace hashed {
namespace slt {


/*! @brief slt::Lattice describes a 3d grid containing
*         slt::LatticeNodes. A slt::Lattice describes a single layer
*         in the construction of a SingleLightTree.
*         It provides the methods to initialise it with both slt::FullLight
*         or slt::NoLight nodes.
*         The lattice coordinate system is similar to world, however it
*         has been clamped to multiples of the NodeSize
*/
class OctreeLayer {
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
  OctreeLayer(const glm::uvec3 origin_in_lattice,
          const unsigned int n_nodes,
          const int depth,
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
  OctreeLayer(const glm::uvec3 origin_in_lattice,
          const unsigned int n_nodes,
          const int depth,
          const float node_size,
          OctreeLayerNode* p_node);

  /*! @brief Construct a new slt::Lattice initialised with NoLight nodes
  */
  OctreeLayer(const glm::uvec3 origin_in_lattice,
              const unsigned int n_nodes,
              const int depth,
              const float node_size,
              const No& no_light);

  /*! @brief Construct a new Lattice initialised with FullLight nodes
  */
  OctreeLayer(const glm::uvec3 origin_in_lattice,
              const unsigned int n_nodes,
              const int depth,
              const float node_size,
              const Full& full_light);

  // --------------------------------------------------------------------------
  /*! @brief Deconstruct this SLTLattice
  */
  ~OctreeLayer();

  // --------------------------------------------------------------------------
  /*! @brief Get the SLTLatticeNode at the given position
  *
  * @param position The position at which the returned SLTLatticeNode resides
  *
  * @return The SLTLatticeNode at position
  */
  const OctreeLayerNode& getOctreeLayerNode(glm::uvec3 position) const;

  /*! @brief Set the SLTLatticeNode at the given position to node.
  *
  * @param position The position at which the node should be placed
  * @param node The node to which the node at position should be set
  *
  * @post this->getLatticeNode(position) == node
  */
  void setOctreeLayerNode(glm::uvec3 position,
                          const OctreeLayerNode& node);

  /*! @brief Set the SLTLatticeNode at the given position to a NoLight node
  *
  * @param position The position at which the node should be placed
  * @param no_light The node to which the SLTLatticeNode should refer
  *
  * @post this->getLatticeNode(position) == SLTLatticeNode(NoLight, no_light)
  */
  void setOctreeLayerNode(glm::uvec3 position,
                          const No& no_light);

  /*! @brief Set the SLTLatticeNode at the given position to a FullLight node
  *
  * @param position The position at which the node should be placed
  * @param full_light The node to which the SLTLatticeNode should refer
  *
  * @post this->getLatticeNode(position) ==
  *        SLTLatticeNode(FullLight, full_light)
  */
  void setOctreeLayerNode(glm::uvec3 position,
                          const Full& full_light);


  /*! @brief Set the SLTLatticeNode at the given position to a PartialLight
  *        node
  *
  * @param position The position at which the node should be placed
  * @param no_light The node to which the SLTLatticeNode should refer
  *
  * @post this->getLatticeNode(position) == S
  *       LTLatticeNode(PartialLight, partial_light)
  */
  void setOctreeLayerNode(glm::uvec3 position,
                          const Partial& partial_light);

  /*! @brief Get the number of nodes in each dimension of this SLTLattice
  *
  * @return The number of nodes in each dimension of this SLTLattice.
  */
  const unsigned int getNNodes() const { return this->n_nodes; }

  /*! @brief Get the origin of this Lattice in respect to Lattice Coordinates
  *
  * @return The origin of this SLTLattice in respect to Lattice Coordinates
  */
  glm::uvec3 getOriginInLattice() const { return this->origin_in_lattice; }

  inline const float getNodeSize() const { return this->node_size; }

  inline const unsigned int getDepth() const { return this->depth; }

  void exportToJson(const std::string& path, glm::vec3 octree_origin) const;

private:
  /*! @brief Set the internal lattice node pointer to the provided pointer
  *
  * @param position position of the node pointer that should be updated
  * @param p_node pointer to the new node that should be placed at the position
  */
  void setOctreeLayerNode(glm::uvec3 position,
                          OctreeLayerNode* p_node);

  /*! @brief Get the internal index of the node at position
  *
  * @return the internal index of the node at position
  */
  int getIndexNode(glm::uvec3 position) const;

  /*! @brief The number of nodes in each dimension of this SLTLattice */
  const unsigned int n_nodes;

  /*! @brief The position of this SLTLattice in Lattice Coordinates */
  const glm::uvec3 origin_in_lattice;

  /*! @brief The depth of this lattice within the LightOctree. */
  const unsigned int depth;

  const float node_size;

  /*! @brief An array of SLTLatticeNode pointers, representing
  *         the internal lattice
  */
  std::vector<OctreeLayerNode*> layer_nodes;
  std::vector<OctreeLayerNode*> unique_nodes;
};

} // slt
} // hierarchical
} // pipeline
} // nTiled

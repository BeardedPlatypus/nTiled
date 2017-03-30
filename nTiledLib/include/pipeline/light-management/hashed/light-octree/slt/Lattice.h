#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glm\glm.hpp>
#include <vector>


// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "world\PointLight.h"
#include "NodeType.h"

namespace nTiled {
namespace pipeline {
namespace hashed {

class Lattice {
public:
  // --------------------------------------------------------------------------
  //  Constructor
  // --------------------------------------------------------------------------
  /*! @brief Construct a new Lattice with the given origin, n_nodes, and 
   *         node_size.
   *
   * @param origin The origin of this Lattice in world coordinates.
   * @param n_nodes_dim The number of nodes in a single dimension within this
   *                    new lattice.
   * @param node_size The size of a single node of this new Lattice.
   */
  Lattice(glm::vec3 origin,
          unsigned int n_nodes_dim,
          double node_size,
          world::PointLight light);


  // --------------------------------------------------------------------------
  //  Node Query methods
  // --------------------------------------------------------------------------
  /*! @brief Get the NodeType at index within this Lattice
   * 
   * @param index The index which should be retrieved within this Lattice
   * 
   * @returns The NodeType associated with the node at index within this 
   *          Lattice
   */
  NodeType getNode(glm::uvec3 index) const;

  /*! @brief Get the index of the node closest to the light source that is both 
   *         within this Lattice and within the specified cube.
   * 
   * @param cube_origin the origin of the cube in which the closest node to the
   *                    light source is retrieved.
   * @param cube_size the size of the cube in which the closest node to the 
   *                  light source is retrieved.
   *
   * @return The index of the closest node to the light source
   */
  glm::uvec3 getClosestNodeToLightSource(glm::vec3 cube_origin,
                                         double cube_size) const;

  /*! @brief Get the index of the node furthest away from the light source that
   *         is both within this Lattice and within the specified cube.
   * 
   * @param cube_origin the origin of the cube in which the furthest node away
   *                    from light source is retrieved.
   * @param cube_size the size of the cube in which the furthest node away from
   *                  the light source is retrieved.
   *
   * @return The index of the furthest node away from the light source
   */
  glm::uvec3 getFurthestNodeAwayFromLightSource(glm::vec3 cube_origin,
                                                double cube_size) const;

  /*! @brief Map the point to its corresponding node and return the index of
   *         this node.
   *
   * @param point The point which should be mapped to a node
   *
   * @returns The index of the node to which point maps
   */
  glm::uvec3 mapToNode(glm::vec3 point) const;

  // --------------------------------------------------------------------------
  //  attribute accessors
  // --------------------------------------------------------------------------
  /*! @brief Get the size of a single node within this Lattice
   *
   * @brief The size of a single node of this Lattice
   */
  double getNodeSize() const { return this->node_size; }

  /*! @brief Get the number of nodes in a single dimension in this Lattice
   *
   * @returns the number of nodes in a single dimension of this Lattice
   */
  unsigned int getNumberNodesDim() const { return this->n_nodes_dim; }

  /*! @brief Get the Origin of this Lattice.
   *
   * @returns The origin of this Lattice.
   */
  glm::vec3 getOrigin() const { return this->origin; }

  /*! @brief Get the width of this Lattice.
   *
   * @returns The width of this Lattice.
   */
  double getWidth() const { return ( this->getNodeSize() * 
                                     this->getNumberNodesDim()); }

  /*! @brief Get the light associated with this Lattice
   *
   * @returns The light associated with this lattice
   */
  const world::PointLight& getLight() const { return this->light; }

  // --------------------------------------------------------------------------
  //  set methods attributes
  // --------------------------------------------------------------------------
  /*! @brief Set the node at coord to node_type
   * 
   * @param coord The coordinate of the node that should be set to node_type
   * @param node_type The new NodeType at coord
   */
  void setNode(glm::uvec3 coord, NodeType node_type);

private:
  // --------------------------------------------------------------------------
  //  attributes
  // --------------------------------------------------------------------------
  /*! @brief The size of a single node within this Lattice. */
  double node_size;

  /*! @brief The number of nodes in a single dimension of this Lattice. */
  unsigned int n_nodes_dim;

  /*! @brief The origin of this Lattice. */
  glm::vec3 origin;

  /*! @brief The point light this Lattice approximates. */
  const world::PointLight light;

  // --------------------------------------------------------------------------
  /*! @brief Nodes of this Lattice. */
  std::vector<NodeType> nodes;
};

}
}
}

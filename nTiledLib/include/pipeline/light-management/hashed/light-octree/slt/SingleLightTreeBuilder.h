#pragma once

// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "world\PointLight.h"
#include "SingleLightTree.h"
#include "Lattice.h"


namespace nTiled {
namespace pipeline {
namespace hashed {

class SingleLightTreeBuilder {
public:
  // --------------------------------------------------------------------------
  //  Constructor
  // --------------------------------------------------------------------------
  /*! @brief Construct a new SingleLightTreeBuilder with the information of 
   *         the octree in which it constructs SingleLightTrees.
   *
   * @param minimum_node_size The size of a minimum single node within a 
   *                          constructed SingleLightTree
   * @param origin_octree The origin of the octree in which SingleLightTrees
   *                      are constructed.
   */
  SingleLightTreeBuilder(const double minimum_node_size,
                         const glm::vec3 origin_octree);

  // --------------------------------------------------------------------------
  //  Get | Set methods
  // --------------------------------------------------------------------------
  /*! @brief Get the origin of the octree within this SingleLightTreeBuilder
   *         constructs SingleLightTrees
   *
   * @return The origin of the octree in which SingleLightTrees will be 
   *         constructed.
   */
  glm::vec3 getOriginOctree() const { return this->origin_octree; }

  /*! @brief Get the minimal node size of nodes within the octree
   *         in which this SingleLightTreeBuilder constructs SingleLightTrees
   *
   * @return The minimal node size of nodes in the octree in which 
   *         SingleLightTrees will be constructed
   */
  double getMinimalNodeSize() const { return this->minimum_node_size; }

  // --------------------------------------------------------------------------
  //  SLT construction methods
  // --------------------------------------------------------------------------
  /*! @brief Check whether the light (partly) covers the specified node.
   * 
   * @param light The light which will be checked
   * @param node_origin the location of the origin of this node
   * @param node_edge The size of the node in a single dimension
   * 
   * @returns true if the light covers node, false otherwise
   */
  bool nodeWithinLight(const world::PointLight& light,
                       glm::vec3 node_origin,
                       double node_edge) const;

  /*! @brief Construct a new Lattice based on the specified light
   * 
   * @param light The light of which a new lattice is constructed
   *
   * @returns The pointer to the newly constructed Lattice
   */
  Lattice* constructLattice(const world::PointLight& light) const;

  /*! @brief Get the maximum size of SingleLightTree
   * 
   * @param light The light of which the maximum SLT size is calculated.
   *
   * @returns The size of the SLT constructed for the specified light
   */
  double getMaxSizeSLT(const world::PointLight& light) const;

  /*! @brief Get the maximum number of mininal nodes of a SingleLightTree
   * 
   * @param light The light of which the maximum SLT size is calculated.
   *
   * @returns The maximum number of SLT nodes of the SLT constructed
   *          for the specified light.
   */
  unsigned int getMaxNNodesSLT(const world::PointLight& light) const;

  /*! @brief Determine the NodeType of the provided node dimensions
   *
   * @param node_origin The origin of the node of which the NodeType should be
   *                    determined.
   * @param node_size The size in a single dimension of the node of which the 
   *                  NodeType should be determined.
   * @param lattice The lattice describing the light to which this SLT belongs
   */
  NodeType determineNodeType(glm::vec3 node_origin,
                             double node_size,
                             const Lattice& lattice) const;

  /*! @brief Construct a new SingleLightTree based on the provided light. 
   * 
   * @param light The light of which the SingleLightOctree is created
   *
   * returns A new SingleLightTree
   */
  SingleLightTree* constructSLT(const world::PointLight& light) const;

private:
  /*! @brief The minimum node size of the octree within this builder creates 
   *         SingleLightTrees
   */
  const double minimum_node_size;

  /*! @brief The origin of the octree within this builder creates 
   *         SingleLightTree 
   */
  const glm::vec3 origin_octree;
  
};

} // hashed
} // pipeline
} // nTiled
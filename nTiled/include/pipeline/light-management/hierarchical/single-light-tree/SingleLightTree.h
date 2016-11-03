/*! @file SingleLightTree.h
 *  @brief SingleLightTree.h contains all the definitions related to the 
 *         classes of a SingleLightTrees
 */
#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "world\PointLight.h"
#include "pipeline\light-management\hierarchical\single-light-tree\SingleLightTreeNodes.h"


namespace nTiled {
namespace pipeline {
namespace hierarchical {

/*! @brief SingleLightTree is an immutable class which contains all
 *         information of a LightTree of a single PointLight
 */
class SingleLightTree {
public:
  /*! @brief Construct this SingleLightTree with the given parameters
   * 
   * @param light The Light of this SingleLightTree
   * @param position The position in relation to grid 
   *                 of size root_tile_size
   * @param root The root SLTNode of this SingleLightTree
   * @param root_tile_size The size of the root tile
   */
  SingleLightTree(const world::PointLight& light,
                  const glm::ivec3 position,
                  const slt::Node& root,
                  const float root_size,
                  slt::FullLightNode const * const p_full_light,
                  slt::NoLightNode const * const p_no_light,
                  const std::vector<slt::PartialLightNode const *> partial_light_nodes);

  /*! @brief Destruct this SingleLightTree
   */
  ~SingleLightTree();

  /*! @brief Get the PointLight of this SingleLightTree
   * 
   * @return The PointLight of this SingleLightTree
   */
  const world::PointLight& getLight() const { return this->light; }

  /*! @brief Get the root SLTNode of this SingleLightTree
   * 
   * @return The root SLTNode of this SingleLightTree
   */
  const slt::Node& getRoot() const { return this->root; }

  /*! @brief Get the position relation to a lattice of 
   *         size this->getRootTileSize()
   *
   * @return The position of this root in relation to a lattice
   *          of size this->getRootSize()
   */ 
  glm::ivec3 getPosition() const { return glm::ivec3(this->position); }

  /*! @brief Get the size of the root node of this SingleLightTree
   *
   * @return The size of the root node of this SingleLightTree
   */
  float getRootSize() const { return this->root_size; }

  void exportToJson(const std::string& path);

private:
  /*! @brief The PointLight of this SingleLightTree. */
  const world::PointLight& light;

  /*! @brief The position of this SingleLightTree within the lattice 
   *         of size of the root
   */
  const glm::ivec3 position;

  /*! @brief The size of the root node of this SingleLightTree */
  float root_size;

  /*! @brief The root SLTNode of this SingleLightTree */
  const slt::Node& root;

  // --------------------------------------------------------------------------
  /*! @brief Pointer to a FullLightnode */
  slt::FullLightNode const * const p_full_light;
  /*! @brief Pointer to a NoLightNode*/
  slt::NoLightNode const * const p_no_light;
  /*! @brief Pointers to all PartialLightNodes */
  const std::vector<slt::PartialLightNode const *> partial_light_nodes;
};


} // hierarchical
} // pipeline
} // nTiled



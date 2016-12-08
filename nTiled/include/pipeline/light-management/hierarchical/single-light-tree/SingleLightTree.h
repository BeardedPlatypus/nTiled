/*! @file SingleLightTree.h
 *  @brief SingleLightTree.h contains all the definitions related to the 
 *         classes of a SingleLightTrees
 */
#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glad\glad.h>


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
                  const GLuint index,
                  const glm::vec3 octree_offset,
                  const glm::uvec3 origin_in_octree,
                  const glm::uvec3 octree_middle,
                  const slt::Node& root,
                  const unsigned int depth,
                  const float minimum_node_size,
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
  inline glm::uvec3 getOriginInOctree() const { return this->origin_in_octree; }

  glm::vec3 getOriginInWorld() const {
    return (this->getOctreeOffset() +
            glm::vec3(this->getOriginInOctree()) * this->minimum_node_size ); 
  }

  glm::uvec3 getMiddleInOctree() const { return glm::uvec3(this->octree_middle); }

  inline float getMinimumNodeSize() const { return this->minimum_node_size; }

  inline glm::vec3 getOctreeOffset() const { return this->octree_offset; }

  inline const unsigned int getDepth() const { return this->depth; }

  /*! @brief Get the size of the root node of this SingleLightTree
   *
   * @return The size of the root node of this SingleLightTree
   */
  //float getRootSize() const { return this->root_size; }

  inline GLuint getLightIndex() const { return this->index; }

  void exportToJson(const std::string& path);

private:
  /*! @brief The PointLight of this SingleLightTree. */
  const world::PointLight& light;
  const GLuint index;

  const glm::uvec3 octree_middle;

  const glm::uvec3 origin_in_octree;
  const glm::vec3 octree_offset;
  const float minimum_node_size;

  /*! @brief The depth of this lattice. */
  const unsigned int depth;

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



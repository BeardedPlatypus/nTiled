#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glad\glad.h>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "world\PointLight.h"

#include "nodes\Node.h"
#include "nodes\Full.h"
#include "nodes\No.h"
#include "nodes\Partial.h"


namespace nTiled {
namespace pipeline {
namespace hashed {

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
                  const glm::uvec3 origin_in_octree,
                  const slt::Node& root,
                  const unsigned int depth,
                  slt::Full const * const p_full_light,
                  slt::No const * const p_no_light,
                  const std::vector<slt::Partial const *> partial_light_nodes);

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


  inline const unsigned int getDepth() const { return this->depth; }

  inline GLuint getLightIndex() const { return this->index; }

  void exportToJson(const std::string& path,
                    unsigned int depth,
                    glm::vec3 octree_origin,
                    float minimum_node_size);

private:
  /*! @brief The PointLight of this SingleLightTree. */
  const world::PointLight& light;
  const GLuint index;

  const glm::uvec3 origin_in_octree;

  /*! @brief The depth of this lattice. */
  const unsigned int depth;

  /*! @brief The root SLTNode of this SingleLightTree */
  const slt::Node& root;

  // --------------------------------------------------------------------------
  /*! @brief Pointer to a FullLightnode */
  slt::Full const * const p_full_light;
  /*! @brief Pointer to a NoLightNode*/
  slt::No const * const p_no_light;
  /*! @brief Pointers to all PartialLightNodes */
  const std::vector<slt::Partial const *> partial_light_nodes;
};


} // hierarchical
} // pipeline
} // nTiled



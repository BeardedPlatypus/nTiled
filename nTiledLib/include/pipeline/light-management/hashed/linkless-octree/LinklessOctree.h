#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <vector>
#include <glad\glad.h>
#include <glm\glm.hpp>


// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "SpatialHashFunction.h"

#include "math\octree.h"

namespace nTiled {
namespace pipeline {
namespace hashed {

class LinklessOctree {
public:
  // --------------------------------------------------------------------------
  //  Constructors | Destructors
  // --------------------------------------------------------------------------
  /*! @brief Construct a new LinklessOctree with the given parameters
   *
   * @param depth The depth of this LinklessOctree
   * @param n_levels The number of levels of this LinklessOctree
   * @param minimum_node_size The size of a single smallest node within this 
   *                          LinklessOctree
   * @param origin The origin of this LinklessOctree
   *
   * @param p_octree_hash_maps Pointer to the vector containing the hash maps 
   *                           describing the structure of this LinklessOctree
   * @param p_data_hash_map_exists Pointer to the vector describing whether a
   *                               level contains data or not.
   * @param p_data_hash_maps Pointer to the vector containing the hash maps 
   *                         describing the data associated with this 
   *                         LinklessOctree
   * @param p_light_indices Pointer to the vector containing the list of light
   *                        indices associated with this LinklessOctree
   */
  LinklessOctree(unsigned int depth,
                 unsigned int n_levels,
                 double minimum_node_size,
                 glm::vec3 origin,
                 std::vector<SpatialHashFunction<glm::u8vec2>*>* p_octree_hash_maps,
                 std::vector<bool>* p_data_hash_map_exists,
                 std::vector<SpatialHashFunction<glm::uvec2>*>* p_data_hash_maps,
                 std::vector<GLuint>* p_light_indices);

  /*! @brief Destruct this LinklessOctree. */
  ~LinklessOctree();

  // --------------------------------------------------------------------------
  //  Get methods
  // --------------------------------------------------------------------------
  /*! @brief Get the origin of this LinklessOctree
   * 
   * @returns The origin of this LinklessOctree
   */
  glm::vec3 getOrigin() const { return this->origin; }

  /*! @brief Get the depth of this LinklessOctree. 
   * 
   * @returns The depth of this LinklessOctree 
   */
  unsigned int getDepth() const { return this->depth; }

  /*! @brief Get the size of the minimum nodes of this LinklessOctree
   *
   * @returns The size of the minimum nodes of this LinklessOctree.
   */
  double getMinimumNodeSize() const { return this->minimum_node_size; }

  /*! @brief Get the number of levels of this LinklessOctree
   * 
   * @returns the number of levels of this LinklessOctree
   */
  unsigned int getNLevels() const { return this->n_levels; }

  /*! @brief Get the total number of nodes of this LinklessOctree
   *        
   * @returns The total number of minimum nodes in a single dimension.
   */
  unsigned int getTotalNNodes() const { 
    return math::calculateNNodes(this->getDepth() - 1);
  }

  /*! @Brief Get the number of nodes of the first level of this LinklessOctree. 
   * 
   * @returns The number of nodes of the first level of this LinklessOctree.
   */
  unsigned int getInitialNNodes() const {
    return math::calculateNNodes(this->getDepth() - (this->getNLevels() + 1));
  }

  /*! @brief Get the total width in a single dimension of this LinklessOctree
   *
   * @returns The total width in a single dimension of this LinklessOctree
   */
  double getWidth() const { 
    return this->getTotalNNodes() * this->getMinimumNodeSize(); 
  }

  // --------------------------------------------------------------------------
  /*! @brief Retrieve the light indices associated with the provided point
   *
   * @param point The point of which the light indices should be retrieved
   *
   * @returns Indices of all lights that potentially effect this point.
   */
  std::vector<GLuint> retrieveLights(glm::vec3 point) const;

  // --------------------------------------------------------------------------
  //  openGL methods
  // --------------------------------------------------------------------------
  /*! @brief Load this linkless octree to the openGLshader specified with
   *         shader_id
   * 
   * @param shader_id The shader to which this octree should be added
   */
  void loadToShader(GLuint shader_id);

private:
  // --------------------------------------------------------------------------
  //  Octree Attributes
  // --------------------------------------------------------------------------
  /*! @brief The depth of this LinklessOctree.
   * @note The depth is specified as the number of levels such that 
   *       n_nodes = 2^(depth - 1)
   */
  unsigned int depth;

  /*! @brief The number of levels this LinklessOctree. contains
   * @note The n_levels is specified such that 
   *       initial number of nodes = 2^(depth - n_levels)
   */
  unsigned int n_levels;

  /*! @brief The minimum node size used in this LinklessOctree. */
  double minimum_node_size;

  /*! @brief The origin of this LinklessOctree in world coordinates. */
  glm::vec3 origin;

  // --------------------------------------------------------------------------
  //  Octree Data
  // --------------------------------------------------------------------------
  /*! @brief The hash maps of this LinklessOctree describing the octree 
   *         structure
   */
  std::vector<SpatialHashFunction<glm::u8vec2>*>* p_octree_hash_maps;

  /*! @brief Vector indicating whether a data hashmap exists for a specific 
   *         level.
   */
  std::vector<bool>* p_data_hash_map_exists;

  /*! @brief The hash maps of this LinklessOctree containing the light data
   *         contains a nullptr if there exists no leaf hash map for that level
   */
  std::vector<SpatialHashFunction<glm::uvec2>*>* p_data_hash_maps;

  /*! @brief The light indices associated with this LinklessOctree. */
  std::vector<GLuint>* p_light_indices;

  // --------------------------------------------------------------------------
  //  openGL specific data
  // --------------------------------------------------------------------------
  // TODO: Integrate this with other datastructures

  /*! @brief The openGL representation of the data tables of p_octree_hash_maps. */
  //GLubyte** p_octree_hash_maps_data_opengl;
  std::vector<std::vector<GLubyte>> gl_octree_data;

  /*! @brief The openGL representation of the offset tables of p_octree_hash_maps. */
  //GLubyte** p_octree_hash_maps_offset_opengl;
  std::vector<std::vector<GLubyte>> gl_octree_offset;

  /*! @brief The openGL representation of the data table of p_data_hash_maps. */
  //GLuint** p_data_hash_maps_data_opengl;
  std::vector<std::vector<GLuint>> gl_light_data;

  /*! @brief The openGL representation of the offset tables of p_data_hash_maps. */
  //GLubyte** p_data_hash_maps_offset_opengl;
  std::vector<std::vector<GLubyte>> gl_light_offset;

  // --------------------------------------------------------------------------
  /*! @brief Array of openGL pointers to the textures used to store p_octree_hash_maps_data_opengl. */
  GLuint* ps_gfx_octree_node_tables;

  /*! @brief Array of openGL pointers to the textures used to store p_octree_hash_maps_offset_opengl. */
  GLuint* ps_gfx_octree_offset_tables;

  /*! @brief Array of openGL pointers to the textures used to store p_data_hash_maps_data_opengl. */
  GLuint* ps_gfx_data_node_tables;

  /*! @brief Array of openGL pointers to the textures used to store p_data_hash_maps_offset_opengl. */
  GLuint* ps_gfx_data_offset_tables;

  /*! @brief OpenGL pointer to the array storing p_light_indices. */
  GLuint p_gfx_light_indices;
};

}
}
}

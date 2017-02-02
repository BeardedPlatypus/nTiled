/*! @file LightClustering.h
 *  @brief LightClustering.h contains the definition of LightClustering class
 *         which serves as datastructure for the Clustered Shading algorithm.
 */
#pragma once

// ----------------------------------------------------------------------------
// Libraries
// ----------------------------------------------------------------------------
#include <glad\glad.h>
#include <glm\glm.hpp>
#include <vector>


namespace nTiled {
namespace pipeline {
namespace clustered {

/*! @brief LightClustering is the core datastructure used in the Clustered 
 *         Shading algorithm. It is responsible for calculating the 
 *         light clustering once and load it into video memory.
 */
class LightClustering {
public:
  // --------------------------------------------------------------------------
  // Constructor | Destructor
  // --------------------------------------------------------------------------
  /*! @brief Construct a new LightClustering with the specified image 
   *         dimensions and tile_size.
   *
   * @param dimensions The screen dimensions in pixels
   * @param tile_size The tile size in pixels used in this LightClustering.
   */
  LightClustering(glm::uvec2 dimensions,
                  glm::uvec2 tile_size);

  // --------------------------------------------------------------------------
  // Frame construction steps
  // --------------------------------------------------------------------------
  /*! @brief Construct a new frame of this LightClustering with the specified 
   *         unique clusters and number of clusters per tile.
   *
   * This function should be called only once per frame, and starts the 
   * process of constructing a new LightClustering for this frame.
   *
   * @param unique_clusters The unique clusters used in the initialisation of
   *                        this frame
   * @param n_clusters_tile Number of clusters per tile.
   */
  void initFrame(const std::vector<GLushort>& unique_clusters,
                 const std::vector<GLushort>& n_clusters_tile);

  /*! @brief Increment all clusters affected by the light with the specified 
   *         light index

   * The clusters are specified by a (sub)frustrum where frustrum_begin 
   * corresponds with the lower left closest point of the frustrum and 
   * frustrum_end corresponds with the upper right deepest point of the 
   * frustrum.
   * 
   * @param frustum_begin The closest lower left corner of the (sub)
   *                      frustum.
   * @param frustum_end The deepest upper right corner of the (sub)
   *                    frustum.
   * @param light_index Index of the light that affects the specified frustum.
   */
  void incrementLight(glm::uvec3 frustrum_begin,
                      glm::uvec3 frustrum_end,
                      GLuint light_index);

  /*! @brief Construct the light_mapping and light_indices of this frame given 
   *         the  internally constructed data.
   *
   * This function should be called only once per frame, and ends the 
   * process of constructing a new LightClustering for this frame.
   */
  void finaliseClusters();

  // --------------------------------------------------------------------------
  // Access variables
  // --------------------------------------------------------------------------
  /*! @brief The mapping of light clusters to light indices. */
  std::vector<glm::uvec2> cluster_to_light_index_map;
  /*! @brief The list of light indices */
  std::vector<GLuint> light_index_list;
 
private:
  /*! @brief Internal array for constructing light clusters. 
   * 
   * { [ (GLushort, [GLuint ...])...],
   * [ (GLushort, [GLuint ...])...],
   * ...
   * }
   */
  std::vector<std::vector<std::pair<GLushort, std::vector<GLuint>>>> cluster_map_raw;

  /*! @brief Image dimensions in pixels of the screen of this LightClustering. */
  const glm::uvec2 image_dimensions;
  /*! @brief Tile size dimensions in pixels of this LightClustering. */
  const glm::uvec2 tile_size;
  /*! The number of tiles in this LightClustering. */
  glm::uvec2 n_tiles;
};

} // Clustered
} // Pipeline
} // nTiled
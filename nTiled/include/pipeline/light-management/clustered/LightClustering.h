#pragma once

// ----------------------------------------------------------------------------
// Libraries
// ----------------------------------------------------------------------------
#include <glad\glad.h>
#include <glm\glm.hpp>
#include <vector>

// ----------------------------------------------------------------------------
// nTiled headers 
// ----------------------------------------------------------------------------

namespace nTiled {
namespace pipeline {
namespace clustered {

class LightClustering {
public:
  // --------------------------------------------------------------------------
  // Constructor | Destructor
  // --------------------------------------------------------------------------
  /*!
   Construct a new LightClustering with the specified image dimensions and 
   tile_size.
   */
  LightClustering(glm::uvec2 dimensions,
                  glm::uvec2 tile_size);
  ~LightClustering();

  // --------------------------------------------------------------------------
  // Frame construction steps
  // --------------------------------------------------------------------------
  /*!
   Construct a new frame of this LightClustering with the specified unique 
   clustern and number of clusters per tile.

   This function should be called only once per frame, and starts the 
   process of constructing a new LightClustering for this frame.
   */
  void initFrame(const std::vector<GLushort>& unique_clusters,
                 const std::vector<GLushort>& n_clusters_tile);

  /*!
   Increment all clusters affected by the light with the specified light index

   The clusters are specified by a (sub)frustrum where frustrum_begin 
   corresponds with the lower left closest point of the frustrum and 
   frustrum_end corresponds with the upper right deepest point of the 
   frustrum.
   */
  void incrementLight(glm::uvec3 frustrum_begin,
                      glm::uvec3 frustrum_end,
                      GLuint light_index);

  /*!
   Construct the light_mapping and light_indices of this frame given the 
   internally constructed data.

   This function should be called only once per frame, and ends the 
   process of constructing a new LightClustering for this frame.
   */
  void finaliseClusters();

  // --------------------------------------------------------------------------
  // Access variables
  // --------------------------------------------------------------------------
  /*! The mapping of light clusters to light indices. */
  std::vector<glm::uvec2> cluster_to_light_index_map;
  /*! The list of light indices */
  std::vector<GLuint> light_index_list;
  
private:
  /*! 
   Internal array for constructing light clusters. 
   { [ (GLushort, [GLuint ...])...],
     [ (GLushort, [GLuint ...])...],
     ...
   }
   */
  std::vector<std::pair<GLushort, std::vector<GLuint>>>* cluster_map_raw;

  const glm::uvec2 image_dimensions;
  const glm::uvec2 tile_size;
  glm::uvec2 n_tiles;
};

}
}
}
/*! @file LightGrid.h 
 *  @brief LightGrid.h contais the definition of the LightGrid class used
 *         in Tiled shading.
 */
#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glad\glad.h>
#include <glm\glm.hpp>
#include <vector>

namespace nTiled {
namespace pipeline {

/*! @brief LightGrid stores both the light index list and light grid which are
 *         constructed by the LightManager.
 */
class LightGrid {
 public:
  // --------------------------------------------------------------------------
  // Constructor
  // --------------------------------------------------------------------------
   /*! @brief Construct a new LightGrid with the given parameters 
    * 
    * @param total_width Total width in pixels of the viewport
    * @param total_height Total height in pixels of the viewport
    * @parm tile_width width in pixels of a single tile in this new LightGrid
    * @param tile_height height in pixels of a single tile in this new LightGrid
    */
  LightGrid(unsigned int total_width, 
            unsigned int total_height,
            unsigned int tile_width, 
            unsigned int tile_height);

  /*! @brief Default destructor for this LightGrid */
  ~LightGrid();

  // --------------------------------------------------------------------------
  // Members
  // --------------------------------------------------------------------------
  /*! @brief Total width in pixels of the viewport. */
  unsigned int total_width;
  /*! @brief Total height in pixels of the viewport. */
  unsigned int total_height;
  /*! @brief width in pixels of a single tile in this LightGrid. */
  unsigned int tile_width;
  /*! @brief height in pixels of a single in this LightGrid. */
  unsigned int tile_height;

  /*! @brief Number of tiles this LightGrid is wide. */
  unsigned int n_x;
  /*! @brief Number of tiles this LightGrid is high. */
  unsigned int n_y;
  /*! @brief Total number of tiles in this LightGrid. */
  unsigned int n_tiles;

  // Construction settings
  /*! @brief Increment tiles running from x to z and y to with light_index
   * 
   * @param tiles Set of tiles to updated, where tiles.xy 
   *              corresponds with the lower left corner and 
   *              tiles.zw corresponds with the upper left corner.
   * @param light_index Light index with which the tiles should be 
   *                    incremented.
   */
  void incrementTiles(glm::uvec4 tiles, 
                      unsigned int light_index);

  /*! @brief Set all grid entries to uvec2(0, 0)
   */
  void clearGrid();

  /*! @brief Set grid to values represented internally
   */
  void finaliseGrid();

  /*! @brief Array of uvec2 objects representing the grid in this LightGrid. */
  glm::uvec2* grid;
  /*! @brief Light index list of this LightGrid. */
  std::vector<GLuint> light_index_list;

 private:
   /*! @brief Raw representation of the Light Index List of this LightGrid. */
  std::vector<GLuint>* light_index_list_raw;
  /*! Total number of light indices stored in this LightGrid. */
  unsigned int total_light_indices;
};

} // pipeline
} // nTiled

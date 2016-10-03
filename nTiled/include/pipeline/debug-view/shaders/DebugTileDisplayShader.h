/*! @file DebugTileDisplayShader.h
 *  @brief DebugTileDisplayShader.h contains the definition of
 *         the DebugTileDisplayShader.
 */
#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <vector>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\debug-view\shaders\DebugShader.h"
#include "pipeline\PipelineObject.h"
#include "pipeline\light-management\tiled\TiledLightManager.h"
#include "pipeline\light-management\tiled\BoxProjector.h"

namespace nTiled {
namespace pipeline {

/*! @brief DebugTileDisplayShaders renders for each tile 
 *         how many lights affect it, where black is zero
 *         and white is light max.
 */
class DebugTileDisplayShader : public DebugShader {
public:
  /*! @brief Construct a new DebugTileDisplayShader with the given parameters
   * 
   * @param z_value The depth at which this DebugTileDisplayShader is rendered
   * @param view A reference to the View that this DebugTileDisplayShader 
   *             observes.
   * @param world A reference to the World that this DebugTileDisplayShader
   *              observes.
   * @param tile_size The tilesize in pixels
   */
  DebugTileDisplayShader(GLfloat z_value,
                         const state::View& view,
                         const world::World& world,
                         glm::uvec2 tile_size);

  /*! @brief Destruct this DebugTileDisplayShader
   */
  ~DebugTileDisplayShader();

  virtual void render() override;

private:
  /*! @brief Draw the  grid lines of between the tiles. */
  void drawGrid();
  /*! @brief Draw the grids representing the number of lights. */
  void drawTiles();

  /*! @brief Reference to the View of this DebugTileDisplayShader. */
  const state::View& view;
  /*! @brief Reference to the State of this DebugTileDisplayShader. */
  const world::World& world;

  /*! @brief BoxProjector used to calculate the position of lights */
  const BoxProjector projector;
  /*! @brief TiledLightManager used to calculate the number of lights 
   *         per tile.
   */
  TiledLightManager tiled_light_manager;

  // --------------------------------------------------------------------------
  //  OpenGL related attributes
  // --------------------------------------------------------------------------
  /*! @brief openGL pointer to the openGL program to render the tiles. */
  GLuint tile_shader_program;
  /*! @brief openGL pointer to the openGL program to render the lines. */
  GLuint grid_shader_program;

  /*! @brief Vector to all the PipelineObjects that represent a tile. 
   */
  std::vector<PipelineObject*> tiles;

  /*! @brief PipelineObject containing the grid lines. */
  PipelineObject* p_grid;
};

} // pipeline
} // nTiled
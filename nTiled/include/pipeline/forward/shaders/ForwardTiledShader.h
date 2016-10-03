/*! @file ForwardTiledShader.h
 *  @brief ForwardTiledShader.h contains the definition of the 
 *         ForwardTiledShader which implements a Forward rendering 
 *         approach in combination with Tiled shading.
 */
#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "ForwardShader.h"
#include "pipeline\light-management\tiled\TiledLightManager.h"
#include "pipeline\light-management\tiled\BoxProjector.h"

namespace nTiled {
namespace pipeline {

/*! @brief ForwardTiledShader implements the Tiled Shading algorithm 
 *         within a Forward pipeline.
 *         It inherits from the ForwardShader.
 */
class ForwardTiledShader : public ForwardShader {
public:
  /*! @brief Construct a new ForwardTiledShader with the given parameters
   * 
   * @param shader_id The ForwardShaderId of this new ForwardTiledShader
   * @param path_vertex_shader Path to the vertex shader of this 
   *                           ForwardTiledShader
   * @param path_fragment_shader Path to the fragment shader of this
   *                             ForwardTiledShader.
   * @param world Reference to the World this ForwardTiledShader observes
   * @param view Reference to the View this ForwardTiledShader observes
   * @param p_output_buffer openGL reference to the output buffer this
   *                        ForwardTiledShader should render to.
   * @param tile_size The tilesize in pixels the Tiled Shading algorithm
   *                  will use.
   * @param light_manager_builder Reference to the TiledLightManagerBuilder
   *                              this ForwardTiledShader will use to 
   *                              obtain a TiledLightManager.
   */
  ForwardTiledShader(ForwardShaderId shader_id,
                     const std::string& path_vertex_shader,
                     const std::string& path_fragment_shader,
                     const world::World& world,
                     const state::View& view,
                     GLint p_output_buffer,
                     glm::uvec2 tile_size,
                     const TiledLightManagerBuilder& light_manager_builder);

  virtual void render() override;

protected:
  // --------------------------------------------------------------------------
  // render sub-functions
  // --------------------------------------------------------------------------
  /*! @brief Load the light grid for this ForwardTiledShader. */
  virtual void loadLightGrid();

  // --------------------------------------------------------------------------
  // LightManagement Functions
  // --------------------------------------------------------------------------
  /*! @brief Sphere projector of this ForwardTiledShader */
  const BoxProjector projector;
  /*! @brief Pointer to TiledLightManager of this ForwardTiledShader */
  TiledLightManager* p_light_manager;

  // --------------------------------------------------------------------------
  //  openGL LightManagement datastructures
  // --------------------------------------------------------------------------
  /*! @brief GLuint pointer to the Shader Storage Buffer Object storing
   *         the light grid of this ForwardTiledShader
   */
  GLuint light_grid_buffer;
  /*! @brief GLuint pointer to the Shader Storage Buffer Object storing
   *         the light index of this ForwardTiledShader
   */
  GLuint light_index_buffer;
};

} // pipeline
} // nTiled

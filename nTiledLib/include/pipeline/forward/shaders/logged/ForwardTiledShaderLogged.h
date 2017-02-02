/*! @file ForwardTiledShaderLogged.h
 *  @brief ForwardTiledShaderLogged.h contains the definition of 
 *         the logged version of ForwardTiledShader.
 */
#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\forward\shaders\ForwardTiledShader.h"
#include "log\Logger.h"


namespace nTiled {
namespace pipeline {

/*! @brief ForwardTiledShaderLogged acts exactly like the 
 *         ForwardTiledShader however it tracks the execution time
 *         of each defined function.
 */
class ForwardTiledShaderLogged : public ForwardTiledShader {
public:
  ForwardTiledShaderLogged(
    ForwardShaderId shader_id,
    const std::string& path_vertex_shader,
    const std::string& path_fragment_shader,
    const world::World& world,
    const state::View& view,
    GLint p_output_buffer,
    glm::uvec2 tile_size,
    const TiledLightManagerBuilder& light_manager_builder,
    logged::ExecutionTimeLogger& logger);

protected:
  virtual void renderObjects() override;
  virtual void loadLightGrid() override;

  /*! @brief Reference to the ExecutionTimeLogger object which logs
   *         the execution time of the constructed ForwardTiledShaderLogged
   */
  logged::ExecutionTimeLogger& logger;
};

} // pipeline
} // nTiled

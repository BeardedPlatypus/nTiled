#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\forward\shaders\ForwardTiledShader.h"
#include "log\LightCalculationsLogger.h"


namespace nTiled {
namespace pipeline {

/*! @brief ForwardTiledShaderLogged acts exactly like the 
 *         ForwardTiledShader however it tracks the execution time
 *         of each defined function.
 */
class ForwardTiledShaderCounted : public ForwardTiledShader {
public:
  ForwardTiledShaderCounted(
    ForwardShaderId shader_id,
    const std::string& path_vertex_shader,
    const std::string& path_fragment_shader,
    const world::World& world,
    const state::View& view,
    GLint p_output_buffer,
    glm::uvec2 tile_size,
    const TiledLightManagerBuilder& light_manager_builder,
    logged::LightCalculationsLogger& logger);

protected:
  virtual void renderObjects() override;

  /*! @brief Reference to the ExecutionTimeLogger object which logs
   *         the execution time of the constructed ForwardTiledShaderLogged
   */
  logged::LightCalculationsLogger& logger;
};

} // pipeline
} // nTiled

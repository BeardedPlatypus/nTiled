#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\forward\shaders\ForwardClusteredShader.h"
#include "log\LightCalculationsLogger.h"


namespace nTiled {
namespace pipeline {

/*! @brief ForwardClusteredShaderLogged acts exactly like the 
 *         ForwardClusteredShader however it tracks the execution time
 *         of each defined function.
 */
class ForwardClusteredShaderCounted : public ForwardClusteredShader {
public:
  ForwardClusteredShaderCounted(
    ForwardShaderId shader_id,
    const std::string& path_vertex_shader,
    const std::string& path_fragment_shader,
    const world::World& world,
    const state::View& view,
    GLint p_output_buffer,
    glm::uvec2 tile_size,
    const ClusteredLightManagerBuilder& light_manager_builder,
    logged::LightCalculationsLogger& logger);

protected:
  virtual void renderObjects() override;

  /*! @brief Reference to the ExecutionTimeLogger object which logs
   *         the execution time of the constructed ForwardClusteredShaderLogged
   */
  logged::LightCalculationsLogger& logger;
};

}
}

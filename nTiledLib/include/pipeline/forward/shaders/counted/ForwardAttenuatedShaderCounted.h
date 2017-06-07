#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\forward\shaders\ForwardAttenuatedShader.h"
#include "log\LightCalculationsLogger.h"


namespace nTiled {
namespace pipeline {

/*! @brief ForwardAttenuatedShaderLogged acts exactly like the 
 *         ForwardAttenuatedShader however it tracks the execution time
 *         of each defined function.
 */
class ForwardAttenuatedShaderCounted : public ForwardAttenuatedShader {
public:
  ForwardAttenuatedShaderCounted(
    ForwardShaderId shader_id,
    const std::string& path_vertex_shader,
    const std::string& path_fragment_shader,
    const world::World& world,
    const state::View& view,
    GLint p_output_buffer,
    logged::LightCalculationsLogger& logger);

protected:
  virtual void renderObjects() override;

  /*! @brief Reference to the ExecutionTimeLogger object which logs
   *         the execution time of the constructed ForwardAttenuatedShaderLogged
   */
  logged::LightCalculationsLogger& logger;
};

} // pipeline
} // nTiled

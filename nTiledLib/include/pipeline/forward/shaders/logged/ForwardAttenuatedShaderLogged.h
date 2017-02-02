/*! @file ForwardAttenuatedShaderLogged.h
 *  @brief ForwardAttenuatedShaderLogged.h contains the definition of 
 *         the logged version of ForwardAttenuatedShader.
 */
#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\forward\shaders\ForwardAttenuatedShader.h"
#include "log\Logger.h"


namespace nTiled {
namespace pipeline {

/*! @brief ForwardAttenuatedShaderLogged acts exactly like the 
 *         ForwardAttenuatedShader however it tracks the execution time
 *         of each defined function.
 */
class ForwardAttenuatedShaderLogged : public ForwardAttenuatedShader {
public:
  ForwardAttenuatedShaderLogged(
    ForwardShaderId shader_id,
    const std::string& path_vertex_shader,
    const std::string& path_fragment_shader,
    const world::World& world,
    const state::View& view,
    GLint p_output_buffer,
    logged::ExecutionTimeLogger& logger);

protected:
  virtual void renderObjects() override;

  /*! @brief Reference to the ExecutionTimeLogger object which logs
   *         the execution time of the constructed ForwardAttenuatedShaderLogged
   */
  logged::ExecutionTimeLogger& logger;
};

} // pipeline
} // nTiled

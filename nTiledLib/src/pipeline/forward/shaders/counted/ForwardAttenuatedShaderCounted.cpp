#include "pipeline\forward\shaders\counted\ForwardAttenuatedShaderCounted.h"

namespace nTiled {
namespace pipeline {

ForwardAttenuatedShaderCounted::ForwardAttenuatedShaderCounted(
    ForwardShaderId shader_id,
    const std::string& path_vertex_shader,
    const std::string& path_fragment_shader,
    const world::World& world,
    const state::View& view,
    GLint p_output_buffer,
    logged::LightCalculationsLogger& logger) :
  ForwardAttenuatedShader(shader_id,
                          path_vertex_shader, 
                          path_fragment_shader,
                          world, 
                          view,
                          p_output_buffer),
  logger(logger) { }

void ForwardAttenuatedShaderCounted::renderObjects() {
  this->logger.preRender();
  ForwardAttenuatedShader::renderObjects();
  this->logger.extractCalculations();
  this->logger.postRender();
}


}
}

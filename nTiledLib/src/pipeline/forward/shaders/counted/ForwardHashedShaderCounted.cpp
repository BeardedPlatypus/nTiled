#include "pipeline\forward\shaders\counted\ForwardHashedShaderCounted.h"

namespace nTiled {
namespace pipeline {

ForwardHashedShaderCounted::ForwardHashedShaderCounted(
    ForwardShaderId shader_id,
    const std::string& path_vertex_shader,
    const std::string& path_fragment_shader,
    const world::World& world,
    const state::View& view,
    GLint p_output_buffer,
    const hashed::HashedLightManagerBuilder& light_manager_builder,
    const hashed::HashedConfig& config,
    logged::LightCalculationsLogger& logger) :
  ForwardHashedShader(shader_id,
                      path_vertex_shader,
                      path_fragment_shader,
                      world,
                      view,
                      p_output_buffer,
                      light_manager_builder,
                      config),
  logger(logger) {
}


void ForwardHashedShaderCounted::renderObjects() {
  this->logger.preRender();
  ForwardHashedShader::renderObjects();
  this->logger.postRender();
  this->logger.extractCalculations();
}


}
}

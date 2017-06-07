#include "pipeline\deferred\shaders\counted\DeferredHashedShaderCounted.h"

namespace nTiled {
namespace pipeline {

DeferredHashedShaderCounted::DeferredHashedShaderCounted(
    DeferredShaderId shader_id,
    const std::string& path_geometry_pass_vertex_shader,
    const std::string& path_geometry_pass_fragment_shader,
    const std::string& path_light_pass_vertex_shader,
    const std::string& path_light_pass_fragment_shader,
    const world::World& world,
    const state::View& view,
    GLint p_output_buffer,
    const hashed::HashedLightManagerBuilder& light_manager_builder,
    const hashed::HashedConfig& config,
    logged::LightCalculationsLogger& logger) :
    DeferredHashedShader(shader_id,
                         path_geometry_pass_vertex_shader,
                         path_geometry_pass_fragment_shader,
                         path_light_pass_vertex_shader,
                         path_light_pass_fragment_shader,
                         world,
                         view,
                         p_output_buffer,
                         light_manager_builder,
                         config),
  logger(logger) {
}



void DeferredHashedShaderCounted::renderLightPassObjects() {
  this->logger.preRender();
  DeferredHashedShader::renderLightPassObjects();
  this->logger.postRender();
  this->logger.extractCalculations();
}


}
}

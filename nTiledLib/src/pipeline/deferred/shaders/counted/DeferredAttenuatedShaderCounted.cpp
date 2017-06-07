#include "pipeline\deferred\shaders\counted\DeferredAttenuatedShaderCounted.h"

namespace nTiled {
namespace pipeline {


DeferredAttenuatedShaderCounted::DeferredAttenuatedShaderCounted(
    DeferredShaderId shader_id,
    const std::string& path_geometry_pass_vertex_shader,
    const std::string& path_geometry_pass_fragment_shader,
    const std::string& path_light_pass_vertex_shader,
    const std::string& path_light_pass_fragment_shader,
    const world::World& world,
    const state::View& view,
    GLint p_output_buffer,
    logged::LightCalculationsLogger& logger) :
  DeferredAttenuatedShader(shader_id,
                           path_geometry_pass_vertex_shader,
                           path_geometry_pass_fragment_shader,
                           path_light_pass_vertex_shader,
                           path_light_pass_fragment_shader,
                           world,
                           view,
                           p_output_buffer),
  logger(logger) {
}


void DeferredAttenuatedShaderCounted::renderLightPassObjects() {
  this->logger.preRender();
  DeferredAttenuatedShader::renderLightPassObjects();
  this->logger.extractCalculations();
  this->logger.postRender();
}

}
}
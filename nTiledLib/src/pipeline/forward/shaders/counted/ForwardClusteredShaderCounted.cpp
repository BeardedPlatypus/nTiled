#include "pipeline\forward\shaders\counted\ForwardClusteredShaderCounted.h"

namespace nTiled {
namespace pipeline {

ForwardClusteredShaderCounted::ForwardClusteredShaderCounted(
    ForwardShaderId shader_id,
    const std::string& path_vertex_shader,
    const std::string& path_fragment_shader,
    const world::World& world,
    const state::View& view,
    GLint p_output_buffer,
    glm::uvec2 tile_size,
    const ClusteredLightManagerBuilder& light_manager_builder,
    logged::LightCalculationsLogger& logger) :
  ForwardClusteredShader(shader_id, 
                         path_vertex_shader,
                         path_fragment_shader,
                         world, view, p_output_buffer, 
                         tile_size, 
                         light_manager_builder),
  logger(logger) {
}

void ForwardClusteredShaderCounted::renderObjects() {
  this->logger.preRender();
  ForwardClusteredShader::renderObjects();
  this->logger.postRender();
  this->logger.extractCalculations();
}


}
}

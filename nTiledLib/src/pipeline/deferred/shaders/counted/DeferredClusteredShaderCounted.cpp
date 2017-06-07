#include "pipeline\deferred\shaders\counted\DeferredClusteredShaderCounted.h"


namespace nTiled {
namespace pipeline {

DeferredClusteredShaderCounted::DeferredClusteredShaderCounted(
  DeferredShaderId shader_id,
  const std::string& path_geometry_pass_vertex_shader,
  const std::string& path_geometry_pass_fragment_shader,
  const std::string& path_light_pass_vertex_shader,
  const std::string& path_light_pass_fragment_shader,
  const world::World& world,
  const state::View& view,
  GLint p_output_buffer,
  glm::uvec2 tile_size,
  const ClusteredLightManagerBuilder& light_manager_builder,
  logged::LightCalculationsLogger& logger) :
  DeferredClusteredShader(shader_id,
                          path_geometry_pass_vertex_shader,
                          path_geometry_pass_fragment_shader,
                          path_light_pass_vertex_shader,
                          path_light_pass_fragment_shader,
                          world,
                          view,
                          p_output_buffer,
                          tile_size,
                          light_manager_builder),
  logger(logger) {
}


void DeferredClusteredShaderCounted::renderLightPassObjects() {
  this->logger.preRender();
  DeferredClusteredShader::renderLightPassObjects();
  this->logger.postRender();
  this->logger.extractCalculations();
}


}
}

#include "pipeline\deferred\shaders\counted\DeferredTiledShaderCounted.h"


namespace nTiled {
namespace pipeline {

DeferredTiledShaderCounted::DeferredTiledShaderCounted(
    DeferredShaderId shader_id,
    const std::string& path_geometry_pass_vertex_shader,
    const std::string& path_geometry_pass_fragment_shader,
    const std::string& path_light_pass_vertex_shader,
    const std::string& path_light_pass_fragment_shader,
    const world::World& world,
    const state::View& view,
    GLint p_output_buffer,
    glm::uvec2 tile_size,
    const TiledLightManagerBuilder& light_manager_builder,
    logged::LightCalculationsLogger& logger) :
  DeferredTiledShader(shader_id, 
                      path_geometry_pass_vertex_shader,
                      path_geometry_pass_fragment_shader,
                      path_light_pass_vertex_shader,
                      path_light_pass_fragment_shader,
                      world,
                      view, 
                      p_output_buffer,
                      tile_size,
                      light_manager_builder),
  logger(logger) { }


void DeferredTiledShaderCounted::renderLightPassObjects() {
  this->logger.preRender();
  DeferredTiledShader::renderLightPassObjects();
  this->logger.extractCalculations();
  this->logger.postRender();
}

} // pipeline
} // nTiled

#include "pipeline\deferred\shaders\logged\DeferredTiledShaderLogged.h"


namespace nTiled {
namespace pipeline {

DeferredTiledShaderLogged::DeferredTiledShaderLogged(
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
    logged::ExecutionTimeLogger& logger) :
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


void DeferredTiledShaderLogged::renderGeometryPassObjects() {
  this->logger.startLog(std::string("DeferredTiledShader::renderGeometryPassObjects"));
  DeferredTiledShader::renderGeometryPassObjects();
  this->logger.endLog();
}

void DeferredTiledShaderLogged::renderLightPassObjects() {
  this->logger.startLog(std::string("DeferredTiledShader::renderLightPassObjects"));
  DeferredTiledShader::renderLightPassObjects();
  this->logger.endLog();
}

void DeferredTiledShaderLogged::loadLightGrid() {
  this->logger.startLog(std::string("DeferredTiledShader::loadLightGrid"));
  DeferredTiledShader::loadLightGrid();
  this->logger.endLog();
}

} // pipeline
} // nTiled
#include "pipeline\forward\shaders\logged\ForwardTiledShaderLogged.h"


namespace nTiled {
namespace pipeline {


ForwardTiledShaderLogged::ForwardTiledShaderLogged(
    ForwardShaderId shader_id,
    const std::string& path_vertex_shader,
    const std::string& path_fragment_shader,
    const world::World& world,
    const state::View& view,
    GLint p_output_buffer,
    glm::uvec2 tile_size,
    const TiledLightManagerBuilder& light_manager_builder,
    logged::ExecutionTimeLogger& logger) : 
  ForwardTiledShader(shader_id,
                     path_vertex_shader,
                     path_fragment_shader,
                     world, 
                     view, 
                     p_output_buffer,
                     tile_size,
                     light_manager_builder),
  logger(logger) {
}

void ForwardTiledShaderLogged::renderObjects() {
  this->logger.startLog(std::string("ForwardTiledShader::renderObjects"));
  ForwardTiledShader::renderObjects();
  this->logger.endLog();
}

void ForwardTiledShaderLogged::loadLightGrid() {
  this->logger.startLog(std::string("ForwardTiledShader::loadLightGrid"));
  ForwardTiledShader::loadLightGrid();
  this->logger.endLog();
}

} // pipeline
} // nTiled
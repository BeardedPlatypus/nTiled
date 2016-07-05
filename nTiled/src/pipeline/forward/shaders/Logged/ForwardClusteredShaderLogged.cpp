#include "pipeline\forward\shaders\logged\ForwardClusteredShaderLogged.h"


namespace nTiled {
namespace pipeline {

ForwardClusteredShaderLogged::ForwardClusteredShaderLogged(
    ForwardShaderId shader_id,
    const std::string& path_vertex_shader,
    const std::string& path_fragment_shader,
    const world::World& world,
    const state::View& view,
    GLint p_output_buffer,
    glm::uvec2 tile_size,
    const ClusteredLightManagerBuilder& light_manager_builder,
    logged::ExecutionTimeLogger& logger) :
  ForwardClusteredShader(shader_id, 
                         path_vertex_shader,
                         path_fragment_shader,
                         world, view, p_output_buffer, 
                         tile_size, 
                         light_manager_builder),
  logger(logger) {
}

void ForwardClusteredShaderLogged::renderObjects() {
  this->logger.startLog(std::string("ForwardClusteredShader::renderObjects"));
  ForwardClusteredShader::renderObjects();
  this->logger.endLog();
}

void ForwardClusteredShaderLogged::depthPass() {
  this->logger.startLog(std::string("ForwardClusteredShader::depthPass"));
  ForwardClusteredShader::depthPass();
  this->logger.endLog();
}

void ForwardClusteredShaderLogged::loadLightClustering() {
  this->logger.startLog(std::string("ForwardClusteredShader::loadLightClustering"));
  ForwardClusteredShader::loadLightClustering();
  this->logger.endLog();
}

}
}
#include "pipeline\deferred\shaders\logged\DeferredClusteredShaderLogged.h"


namespace nTiled {
namespace pipeline {

DeferredClusteredShaderLogged::DeferredClusteredShaderLogged(
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
  logged::ExecutionTimeLogger& logger) :
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

void DeferredClusteredShaderLogged::renderGeometryPassObjects() {
  this->logger.startLog(std::string("DeferredClusteredShader::renderGeometryPassObjects"));
  DeferredClusteredShader::renderGeometryPassObjects();
  this->logger.endLog();
}

void DeferredClusteredShaderLogged::renderLightPassObjects() {
  this->logger.startLog(std::string("DeferredClusteredShader::renderLightPassObjects"));
  DeferredClusteredShader::renderLightPassObjects();
  this->logger.endLog();
}

void DeferredClusteredShaderLogged::loadLightClustering() {
  this->logger.startLog(std::string("DeferredClusteredShader::loadLightClustering"));
  DeferredClusteredShader::loadLightClustering();
  this->logger.endLog();
}

}
}
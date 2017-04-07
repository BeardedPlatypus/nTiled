#include "pipeline\deferred\shaders\logged\DeferredHashedShaderLogged.h"


namespace nTiled {
namespace pipeline {

DeferredHashedShaderLogged::DeferredHashedShaderLogged(
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
    logged::ExecutionTimeLogger& logger) :
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


void DeferredHashedShaderLogged::renderGeometryPassObjects() {
  this->logger.startLog(std::string("DeferredHashedShader::renderGeometryPassObjects"));
  DeferredHashedShader::renderGeometryPassObjects();
  this->logger.endLog();
}

void DeferredHashedShaderLogged::renderLightPassObjects() {
  this->logger.startLog(std::string("DeferredHashedShader::renderLightPassObjects"));
  DeferredHashedShader::renderLightPassObjects();
  this->logger.endLog();
}


}
}
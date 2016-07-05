#include "pipeline\deferred\shaders\logged\DeferredAttenuatedShaderLogged.h"

namespace nTiled {
namespace pipeline {

DeferredAttenuatedShaderLogged::DeferredAttenuatedShaderLogged(
    DeferredShaderId shader_id,
    const std::string& path_geometry_pass_vertex_shader,
    const std::string& path_geometry_pass_fragment_shader,
    const std::string& path_light_pass_vertex_shader,
    const std::string& path_light_pass_fragment_shader,
    const world::World& world,
    const state::View& view,
    GLint p_output_buffer,
    logged::ExecutionTimeLogger& logger) :
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

void DeferredAttenuatedShaderLogged::renderGeometryPassObjects() {
  this->logger.startLog(std::string("DeferredAttenuatedShader::renderGeometryPassObjects"));
  DeferredAttenuatedShader::renderGeometryPassObjects();
  this->logger.endLog();
}

void DeferredAttenuatedShaderLogged::renderLightPassObjects() {
  this->logger.startLog(std::string("DeferredAttenuatedShader::renderLightPassObjects"));
  DeferredAttenuatedShader::renderLightPassObjects();
  this->logger.endLog();
}

}
}
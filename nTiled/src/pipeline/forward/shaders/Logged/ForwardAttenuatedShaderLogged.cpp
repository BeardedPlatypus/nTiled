#include "pipeline\forward\shaders\logged\ForwardAttenuatedShaderLogged.h"

namespace nTiled {
namespace pipeline {

ForwardAttenuatedShaderLogged::ForwardAttenuatedShaderLogged(
    ForwardShaderId shader_id,
    const std::string& path_vertex_shader,
    const std::string& path_fragment_shader,
    const world::World& world,
    const state::View& view,
    GLint p_output_buffer,
    logged::ExecutionTimeLogger& logger) :
  ForwardAttenuatedShader(shader_id,
                          path_vertex_shader, 
                          path_fragment_shader,
                          world, 
                          view,
                          p_output_buffer),
  logger(logger) { }

void ForwardAttenuatedShaderLogged::renderObjects() {
  this->logger.startLog(std::string("ForwardAttenuatedShader::renderObjects"));
  ForwardAttenuatedShader::renderObjects();
  this->logger.endLog();
}


}
}
#include "pipeline\deferred\shaders\DeferredAttenuatedShader.h"

namespace nTiled {
namespace pipeline {

DeferredAttenuatedShader::DeferredAttenuatedShader(
    DeferredShaderId shader_id,
    const std::string& path_geometry_pass_vertex_shader,
    const std::string& path_geometry_pass_fragment_shader,
    const std::string& path_light_pass_vertex_shader,
    const std::string& path_light_pass_fragment_shader,
    const world::World& world,
    const state::View& view) : DeferredShader(shader_id,
                                              path_geometry_pass_vertex_shader,
                                              path_geometry_pass_fragment_shader,
                                              path_light_pass_vertex_shader,
                                              path_light_pass_fragment_shader,
                                              world,
                                              view) { }

void DeferredAttenuatedShader::renderGeometryPass() {
  glUseProgram(this->geometry_pass_sp);
  this->renderGeometryPassObjects();
  glUseProgram(0);
}

void DeferredAttenuatedShader::renderLightPass() {
  glUseProgram(this->light_pass_sp);
  this->renderLightPassObjects();
  glUseProgram(0);
}

}
}
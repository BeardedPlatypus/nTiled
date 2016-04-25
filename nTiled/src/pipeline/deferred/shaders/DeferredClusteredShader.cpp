#include "pipeline\deferred\shaders\DeferredClusteredShader.h"

namespace nTiled {
namespace pipeline {

DeferredClusteredShader::DeferredClusteredShader(
    DeferredShaderId shader_id,
    const std::string& path_geometry_pass_vertex_shader,
    const std::string& path_geometry_pass_fragment_shader,
    const std::string& path_light_pass_vertex_shader,
    const std::string& path_light_pass_fragment_shader,
    const world::World& world,
    const state::View& view,
    GLint p_output_buffer,
    glm::uvec2 tile_size) : DeferredShader(shader_id,
                                            path_geometry_pass_vertex_shader,
                                            path_geometry_pass_fragment_shader,
                                            path_light_pass_vertex_shader,
                                            path_light_pass_fragment_shader,
                                            world,
                                            view,
                                            p_output_buffer) { 
  this->p_shader = new clustered::KeyComputeShader(
    this->gBuffer.getPointerDepthTexture(),
    view,
    glm::uvec2(32, 32));
}

DeferredClusteredShader::~DeferredClusteredShader() {
  delete this->p_shader;
}

void DeferredClusteredShader::renderGeometryPass() {
  glUseProgram(this->geometry_pass_sp);
  this->renderGeometryPassObjects();
  glUseProgram(0);
}

void DeferredClusteredShader::renderLightPass() {
  glUseProgram(this->light_pass_sp);
  //this->renderLightPassObjects();
  p_shader->execute();
  p_shader->debugVisualise();
  glUseProgram(0);
}

}
}

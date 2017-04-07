#include "pipeline\deferred\shaders\DeferredHashedShader.h"

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <iostream>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\shader-util\LoadShaders.h"

namespace nTiled {
namespace pipeline {

DeferredHashedShader::DeferredHashedShader(
    DeferredShaderId shader_id,
    const std::string& path_geometry_pass_vertex_shader,
    const std::string& path_geometry_pass_fragment_shader,
    const std::string& path_light_pass_vertex_shader,
    const std::string& path_light_pass_fragment_shader,
    const world::World& world,
    const state::View& view,
    GLint p_output_buffer,
    const hashed::HashedLightManagerBuilder& light_manager_builder,
    const hashed::HashedConfig& config) :
  DeferredShader(shader_id,
                 path_geometry_pass_vertex_shader,
                 path_geometry_pass_fragment_shader,
                 path_light_pass_vertex_shader,
                 path_light_pass_fragment_shader,
                 world,
                 view,
                 p_output_buffer),
  p_light_manager(light_manager_builder.constructNewHashedLightManager(world,
                                                                       config)) {
  this->p_light_manager->init();
  glUseProgram(this->light_pass_sp);

  this->p_light_manager->getLinklessOctree()->loadToShader(this->light_pass_sp);
  glUseProgram(0);
}


DeferredHashedShader::~DeferredHashedShader() {
  delete this->p_light_manager;
}


void DeferredHashedShader::renderGeometryPass() {
  glUseProgram(this->geometry_pass_sp);
  this->renderGeometryPassObjects();
  glUseProgram(0);
}


void DeferredHashedShader::renderLightPass() {
  glUseProgram(this->light_pass_sp);
  // upload inv_camera_matrix
  glm::mat4 inv_camera_matrix = glm::inverse(view.camera.getLookAt());
  GLint p_inv_camera = glGetUniformLocation(this->light_pass_sp,
                                            "inv_camera_matrix");
  glUniformMatrix4fv(p_inv_camera,
                     1,
                     GL_FALSE,
                     glm::value_ptr(inv_camera_matrix));

  this->renderLightPassObjects();
  glUseProgram(0);
}


void DeferredHashedShader::loadShaders(const std::string& path_geometry_vert_shader,
                                       const std::string& path_geometry_frag_shader,
                                       const std::string& path_light_vert_shader,
                                       const std::string& path_light_frag_shader) {
  // -------------
  // Geometry pass
  // -------------
  // Vertex Shader
  // -----------------------------------------------------------------
  std::stringstream geometry_vert_shader_buffer =
    readShader(path_geometry_vert_shader);
  GLuint geometry_vert_shader = compileShader(GL_VERTEX_SHADER,
                                              geometry_vert_shader_buffer.str());

  // Fragment Shader
  // -----------------------------------------------------------------
  std::stringstream geometry_frag_shader_buffer = 
    readShader(path_geometry_frag_shader);

  GLuint geometry_frag_shader = compileShader(GL_FRAGMENT_SHADER,
                                              geometry_frag_shader_buffer.str());
  this->geometry_pass_sp = createProgram(geometry_vert_shader, geometry_frag_shader);

  // -------------
  // Light pass
  // -------------
  // Vertex Shader
  // -----------------------------------------------------------------
  std::stringstream light_vert_shader_buffer =
    readShader(path_light_vert_shader);
  GLuint light_vert_shader = compileShader(GL_VERTEX_SHADER,
                                           light_vert_shader_buffer.str());

  // Fragment Shader
  // -----------------------------------------------------------------
  std::stringstream light_frag_shader_buffer = 
    readShaderWithLightsAndOctreeMaps(path_light_frag_shader,
                                      this->world.p_lights.size(),
                                      this->p_light_manager->getLinklessOctree()->getNLevels());

  GLuint light_frag_shader = compileShader(GL_FRAGMENT_SHADER,
                                           light_frag_shader_buffer.str());
  this->light_pass_sp = createProgram(light_vert_shader, light_frag_shader);
}


}
}
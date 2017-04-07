#include "pipeline\forward\shaders\ForwardHashedShader.h"

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

ForwardHashedShader::ForwardHashedShader(
    ForwardShaderId shader_id,
    const std::string& path_vertex_shader,
    const std::string& path_fragment_shader,
    const world::World& world,
    const state::View& view,
    GLint p_output_buffer,
    const hashed::HashedLightManagerBuilder& light_manager_builder,
    const hashed::HashedConfig& config) :
    ForwardShader(shader_id, world, view, p_output_buffer),
    p_light_manager(light_manager_builder.constructNewHashedLightManager(world, config)) {

  this->p_light_manager->init();

  this->loadShaders(path_vertex_shader, path_fragment_shader);
  this->loadObjects();
  this->loadLights();

  glm::mat4 perspective_matrix = view.camera.getPerspectiveMatrix();
  GLint p_camera_to_clip = glGetUniformLocation(this->shader,
                                                "camera_to_clip");

  glUseProgram(this->shader);
  glUniformMatrix4fv(p_camera_to_clip,
                     1,
                     GL_FALSE,
                     glm::value_ptr(perspective_matrix));

  this->p_light_manager->getLinklessOctree()->loadToShader(this->shader);

  glUseProgram(0);
  glEnable(GL_DEPTH_TEST);  //TODO: check if this is right place
}


void ForwardHashedShader::loadShaders(const std::string& path_vert_shader,
                                      const std::string& path_frag_shader) {
  // Vertex Shader
  // --------------------------------------------------------------------------
  std::stringstream vert_shader_buffer = readShader(path_vert_shader);
  GLuint vert_shader = compileShader(GL_VERTEX_SHADER,
                                     vert_shader_buffer.str());

  // Fragment Shader
  // --------------------------------------------------------------------------
  std::stringstream frag_shader_buffer = 
    readShaderWithLightsAndOctreeMaps(path_frag_shader,
                                      this->world.p_lights.size(),
                                      this->p_light_manager->getLinklessOctree()->getNLevels());

  GLuint frag_shader = compileShader(GL_FRAGMENT_SHADER,
                                     frag_shader_buffer.str());
  this->shader = createProgram(vert_shader, frag_shader);
}


void ForwardHashedShader::render() {
  glUseProgram(this->shader);
  this->renderObjects();
  glUseProgram(0);
}


}
}
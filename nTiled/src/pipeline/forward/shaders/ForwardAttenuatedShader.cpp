#include "pipeline\forward\shaders\ForwardAttenuatedShader.h"

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

ForwardAttenuatedShader::ForwardAttenuatedShader(
    ForwardShaderId shader_id,
    const std::string& path_vertex_shader,
    const std::string& path_fragment_shader,
    const world::World& world,
    const state::View& view) : ForwardShader(shader_id,
                                             path_vertex_shader,
                                             path_fragment_shader,
                                             world,
                                             view) { }

void ForwardAttenuatedShader::render() const {
  glUseProgram(this->shader);
  glm::mat4 lookAt = this->view.camera.getLookAt();

  GLint p_modelToCamera = glGetUniformLocation(this->shader,
                                               "model_to_camera");

  for (PipelineObject* p_obj : this->ps_obj) {
    glm::mat4 model_to_camera = lookAt * (*p_obj).transformation_matrix;

    glUniformMatrix4fv(p_modelToCamera,
                       1,
                       GL_FALSE,
                       glm::value_ptr(model_to_camera));

    const GLuint vao = (*p_obj).vao;

    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                 (*p_obj).element_buffer);
    glDrawElements(GL_TRIANGLES,
                   (*p_obj).n_elements,
                   GL_UNSIGNED_INT,
                   0);
  }
  glUseProgram(0);
}

} // pipeline
} // nTiled

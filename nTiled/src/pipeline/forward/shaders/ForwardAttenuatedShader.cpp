#include "pipeline\forward\shaders\ForwardAttenuatedShader.h"

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glm/gtc/type_ptr.hpp>

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

void ForwardAttenuatedShader::render() {
  glUseProgram(this->shader);
  glm::mat4 lookAt = this->view.camera.getLookAt();

  GLint p_modelToCamera = glGetUniformLocation(this->shader,
                                               "model_to_camera");

  for (PipelineObject* p_obj : this->ps_obj) {
    // Update model to camera
    // ----------------------
    glm::mat4 model_to_camera = lookAt * p_obj->transformation_matrix;

    glUniformMatrix4fv(p_modelToCamera,
                       1,
                       GL_FALSE,
                       glm::value_ptr(model_to_camera));

    // Update light locations
    // ----------------------
    if (this->lights.size() > 0) {
      glm::mat4 world_to_model = glm::inverse(p_obj->transformation_matrix);
      glBindBuffer(GL_UNIFORM_BUFFER, this->light_ubo);

      for (GLuint i = 0; i < this->lights.size(); i++) {
        glm::vec4 light_model_coordinates =
          world_to_model * this->lights[i].positionCoordinates;
        glBufferSubData(GL_UNIFORM_BUFFER,
                        sizeof(this->lights[0]) * i,
                        sizeof(light_model_coordinates),
                        glm::value_ptr(light_model_coordinates));
      }
      glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    // Render object
    // -------------
    glBindVertexArray(p_obj->vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                 p_obj->element_buffer);
    glDrawElements(GL_TRIANGLES,
                   p_obj->n_elements,
                   GL_UNSIGNED_INT,
                   0);
  }
  glBindVertexArray(0);
  glUseProgram(0);
}

} // pipeline
} // nTiled

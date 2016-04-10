#include "pipeline\forward\shaders\ForwardShader.h"
 
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

ForwardShader::ForwardShader(ForwardShaderId shader_id,
                             const std::string& path_vertex_shader,
                             const std::string& path_fragment_shader,
                             const world::World& world,
                             const state::View& view
                             ) :
    id(shader_id),
    world(world),
    view(view) {
  // load shader
  this->loadShaders(path_vertex_shader,
                    path_fragment_shader);

  // set up objects
  this->loadObjects();

  // set up lights
  this->loadLights();

  // set up misc values
  glm::mat4 perspective_matrix = view.camera.getPerspectiveMatrix();
  GLint p_camera_to_clip = glGetUniformLocation(this->shader,
                                                "camera_to_clip");

  glUseProgram(this->shader);
  glUniformMatrix4fv(p_camera_to_clip,
                     1,
                     GL_FALSE,
                     glm::value_ptr(perspective_matrix));
  glUseProgram(0);
}

void ForwardShader::render() {

}

void ForwardShader::loadObjects() {
  for (world::Object* p_obj : this->world.p_objects) {
    if (p_obj->shader_key.forward_id == this->getId()) {
      // ----------------------------------------------------------------------
      //  Construct new PipelineObject and construct the appropriate buffers
      // ----------------------------------------------------------------------
      // setup buffers
      GLuint vbo_handles[4];
      glGenBuffers(4, vbo_handles);
      GLuint position_buffer = vbo_handles[0];
      GLuint normal_buffer = vbo_handles[1];
      GLuint uv_buffer = vbo_handles[2];
      GLuint element_buffer = vbo_handles[3];
      GLuint vao;

      // setup vertex array object
      glGenVertexArrays(1, &(vao));
      glBindVertexArray(vao);
      glEnableVertexAttribArray(0);
      glEnableVertexAttribArray(1);
      glEnableVertexAttribArray(2);
      glEnableVertexAttribArray(3);

      // set up position buffer
      glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
      glBufferData(GL_ARRAY_BUFFER,
                   p_obj->mesh.vertices.size() * sizeof(glm::vec4), //* 4 * sizeof(GLfloat),
                   &(p_obj->mesh.vertices[0]),
                   GL_STATIC_DRAW);
      glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

      // set up normal buffer
      glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
      if ((*p_obj).mesh.normals.size() > 0) {
        glBufferData(GL_ARRAY_BUFFER,
                     p_obj->mesh.normals.size() * sizeof(glm::vec3), //* 4 * sizeof(GLfloat),
                     &(p_obj->mesh.normals[0]),
                     GL_STATIC_DRAW);
      }
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

      // set up uvw buffer  
      glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
      if ((*p_obj).mesh.uvs.size() > 0) {
        glBufferData(GL_ARRAY_BUFFER,
                     p_obj->mesh.uvs.size() * sizeof(glm::vec3), //* 4 * sizeof(GLfloat),
                     &(p_obj->mesh.uvs[0]),
                     GL_STATIC_DRAW);
      }
      glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

      // set up element buffer
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                   p_obj->mesh.elements.size() * sizeof(glm::tvec3<glm::u32>), // * 3 * sizeof(GLuint),
                   &(p_obj->mesh.elements[0]),
                   GL_STATIC_DRAW);
      glVertexAttribPointer(3, 3, GL_UNSIGNED_INT, GL_FALSE, 0, NULL);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

      this->constructPipelineObject(*p_obj,
                                    vao,
                                    element_buffer);
    }
  }
}

void ForwardShader::constructPipelineObject(const world::Object& obj,
                                            GLuint vao,
                                            GLuint element_buffer) {
      PipelineObject* p_pipeline_obj = new PipelineObject(vao,
                                                          element_buffer,
                                                          obj.mesh.elements.size() * 3,
                                                          obj.transformation_matrix);
      this->ps_obj.push_back(p_pipeline_obj);
}

void ForwardShader::loadLights() {
  // setup light information
  // --------------------------------------------------------------------------
  //  Generate light data
  for (world::PointLight* p_light : this->world.p_lights) {
    this->constructPipelineLight(*p_light);
  }

  glUseProgram(this->shader);
  // ------------------------------------------------------------------------
  // generate UBO
  GLuint num_lights = this->lights.size();
  if (num_lights > 0) {
    // Generate ubo buffer
    glGenBuffers(1, &this->light_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER,
                 this->light_ubo);

    glBufferData(GL_UNIFORM_BUFFER,
                 sizeof((this->lights[0])) * num_lights,
                 this->lights.data(),
                 GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // link UBO with the shader
    GLint p_lightBlock = glGetUniformBlockIndex(this->shader,
                                                "LightBlock");
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->light_ubo);
    glUniformBlockBinding(this->shader,
                          p_lightBlock,
                          0);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
  }

  glUseProgram(0);
}

void ForwardShader::constructPipelineLight(const world::PointLight& light) {
    PipelineLight data = { light.position,
                           light.intensity,
                           light.radius,
                           light.is_emitting };
    this->lights.push_back(data);
}

void ForwardShader::renderObjects() {
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
}

void ForwardShader::loadShaders(const std::string& path_vert_shader,
                                const std::string& path_frag_shader) {
  // Vertex Shader
  // -----------------------------------------------------------------
  std::stringstream vert_shader_buffer =
    readShader(path_vert_shader);
  GLuint vert_shader = compileShader(GL_VERTEX_SHADER,
                                     vert_shader_buffer.str());

  // Fragment Shader
  // -----------------------------------------------------------------
  std::stringstream frag_shader_buffer = readShader(path_frag_shader);

  GLuint frag_shader = compileShader(GL_FRAGMENT_SHADER,
                                     frag_shader_buffer.str());
  this->shader = createProgram(vert_shader, frag_shader);
}

}
}
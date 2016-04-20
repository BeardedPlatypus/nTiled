#include "pipeline\deferred\shaders\DeferredShader.h"

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <fstream>
#include <sstream>
#include <iostream>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\shader-util\LoadShaders.h"
#include "pipeline\pipeline-util\ConstructQuad.h"

namespace nTiled {
namespace pipeline {

DeferredShader::DeferredShader(DeferredShaderId shader_id,
                               const std::string& path_geometry_vert_shader,
                               const std::string& path_geometry_frag_shader,
                               const std::string& path_light_vert_shader,
                               const std::string& path_light_frag_shader,
                               const world::World& world,
                               const state::View& view,
                               GLint p_output_buffer) :
    id(shader_id),
    world(world),
    view(view),
    gBuffer(GBuffer(view.viewport.x,
                    view.viewport.y)),
    p_output_buffer(p_output_buffer) {
  this->loadShaders(path_geometry_vert_shader,
                    path_geometry_frag_shader,
                    path_light_vert_shader,
                    path_light_frag_shader);

  // set up objects
  this->loadObjects();

  // set up lights
  this->loadLights();

  // construct viewplane quad
  this->fullscreen_quad = constructQuad();

  // set up misc values
  glm::mat4 perspective_matrix = view.camera.getPerspectiveMatrix();
  glm::mat4 inverse_perspective_matrix = glm::inverse(perspective_matrix);

  // Geometry pass
  // -------------
  GLint p_camera_to_clip = glGetUniformLocation(this->geometry_pass_sp,
                                                "camera_to_clip");

  glUseProgram(this->geometry_pass_sp);
  glUniformMatrix4fv(p_camera_to_clip,
                     1,
                     GL_FALSE,
                     glm::value_ptr(perspective_matrix));
  glUseProgram(0);


  // Light pass
  // ----------
  GLint p_perspective_matrix = glGetUniformLocation(this->light_pass_sp,
                                                    "perspective_matrix");
  GLint p_inv_perspective_matrix = glGetUniformLocation(this->light_pass_sp,
                                                        "inv_perspective_matrix");

  GLint p_viewport = glGetUniformLocation(this->light_pass_sp,
                                          "viewport");
  GLint p_depthrange = glGetUniformLocation(this->light_pass_sp,
                                            "depth_range");

  glm::vec4 viewport = glm::vec4(0.0f, 0.0f, 
                                 this->view.viewport.x,
                                 this->view.viewport.y);
  glm::vec2 depthrange = this->view.camera.getDepthrange();

  glUseProgram(this->light_pass_sp);
  glUniformMatrix4fv(p_perspective_matrix,
                     1, GL_FALSE,
                     glm::value_ptr(perspective_matrix));
  glUniformMatrix4fv(p_inv_perspective_matrix,
                     1, GL_FALSE,
                     glm::value_ptr(inverse_perspective_matrix));

  glUniform4fv(p_viewport, 1, glm::value_ptr(viewport));
  glUniform2fv(p_depthrange, 1, glm::value_ptr(depthrange));
  glUseProgram(0);

  // Set up GBuffer
  // --------------
  GLint p_diffuse_texture = glGetUniformLocation(this->light_pass_sp,
                                                 "diffuse_tex");
  GLint p_normal_texture = glGetUniformLocation(this->light_pass_sp,
                                                "normal_tex");
  GLint p_depth_texture = glGetUniformLocation(this->light_pass_sp,
                                               "depth_tex");

  glUseProgram(this->light_pass_sp);
  glUniform1i(p_diffuse_texture,
              GL_TEXTURE0);// +this->gBuffer.GBUFFER_TEXTURE_TYPE_DIFFUSE);
  glUniform1i(p_normal_texture,
              GL_TEXTURE1);//GL_TEXTURE0 + this->gBuffer.GBUFFER_TEXTURE_TYPE_NORMAL);
  glUniform1i(p_depth_texture,
              GL_TEXTURE2);//GL_TEXTURE0 + this->gBuffer.GBUFFER_NUM_TEXTURES);

  glUseProgram(0);
}

void DeferredShader::render() {
  // enable writing to gBuffer
  this->gBuffer.bindForWriting();
  glEnable(GL_DEPTH_TEST);

  // clear old contents gBuffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  this->renderGeometryPass();
  glDisable(GL_DEPTH_TEST);
  //this->gBuffer.unbindForWriting();

  // set output to the specified buffer
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->p_output_buffer);

  // bind the texture for reading
  this->gBuffer.bindForReading();

  this->renderLightPass();
  //this->renderBuffers();
}

void DeferredShader::loadObjects() {
  for (world::Object* p_obj : this->world.p_objects) {
    if (p_obj->shader_key.deferred_id == this->getId()) {
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

void DeferredShader::constructPipelineObject(const world::Object& obj,
                                            GLuint vao,
                                            GLuint element_buffer) {
      PipelineObject* p_pipeline_obj = new PipelineObject(vao,
                                                          element_buffer,
                                                          obj.mesh.elements.size() * 3,
                                                          obj.transformation_matrix);
      this->ps_obj.push_back(p_pipeline_obj);
}


void DeferredShader::loadLights() {
  // setup light information
  // --------------------------------------------------------------------------
  //  Generate light data
  for (world::PointLight* p_light : this->world.p_lights) {
    this->constructPipelineLight(*p_light);
  }

  glUseProgram(this->light_pass_sp);
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
    GLint p_lightBlock = glGetUniformBlockIndex(this->light_pass_sp,
                                                "LightBlock");
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->light_ubo);
    glUniformBlockBinding(this->light_pass_sp,
                          p_lightBlock,
                          0);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
  }

  glUseProgram(0);
}

void DeferredShader::constructPipelineLight(const world::PointLight& light) {
    PipelineLight data = { light.position,
                           light.intensity,
                           light.radius,
                           light.is_emitting };
    this->lights.push_back(data);
}

void DeferredShader::renderGeometryPass() {

}

void DeferredShader::renderGeometryPassObjects() {
  glm::mat4 lookAt = this->view.camera.getLookAt();
  for (PipelineObject* obj : this->ps_obj) {
    glBindVertexArray(obj->vao);

    // retrieve memory locations shared per model
    GLint p_model_to_camera = glGetUniformLocation(this->geometry_pass_sp,
                                                   "model_to_camera");
    glm::mat4 model_to_camera = lookAt * obj->transformation_matrix;
    glUniformMatrix4fv(p_model_to_camera, 
                       1, 
                       GL_FALSE,
                       glm::value_ptr(model_to_camera));

    GLint p_normal_model_to_camera = 
      glGetUniformLocation(this->geometry_pass_sp,
                           "inv_transpose_model_to_camera");
    glm::mat4 invTransModelToCamera = glm::inverseTranspose(model_to_camera);
    glUniformMatrix4fv(p_normal_model_to_camera, 
                       1, 
                       GL_FALSE,
                       glm::value_ptr(invTransModelToCamera));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                 obj->element_buffer);
    glDrawElements(GL_TRIANGLES,
                   obj->n_elements,
                   GL_UNSIGNED_INT, 
                   0);
  }
}

void DeferredShader::renderLightPass() {

}

void DeferredShader::renderLightPassObjects() {
  // Update light positions
  glm::mat4 lookAt = this->view.camera.getLookAt();

  glBindBuffer(GL_UNIFORM_BUFFER, this->light_ubo);
  for (GLuint i = 0; i < this->lights.size(); i++) {
    glm::vec4 light_camera_coordinates =
      lookAt * this->lights[i].positionCoordinates;

    glBufferSubData(GL_UNIFORM_BUFFER,
                    sizeof(this->lights[0]) * i,
                    sizeof(light_camera_coordinates),
                    glm::value_ptr(light_camera_coordinates));
  }

  glBindBuffer(GL_UNIFORM_BUFFER, 0);
  // Render elements
  glBindVertexArray(this->fullscreen_quad->vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
               this->fullscreen_quad->element_buffer);
  glDrawElements(GL_TRIANGLES,
                 this->fullscreen_quad->n_elements,
                 GL_UNSIGNED_SHORT, 
                 0);
}

void DeferredShader::renderBuffers() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  GLuint width = this->view.viewport.x;
  GLuint height = this->view.viewport.y;
  GLsizei HalfWidth = (GLsizei)(width / 2.0f);
  GLsizei HalfHeight = (GLsizei)(height / 2.0f);

  this->gBuffer.setReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER,
                    0);
  glBlitFramebuffer(0, 0, width, height,
                    0, HalfHeight, HalfWidth, height,
                    GL_COLOR_BUFFER_BIT, GL_LINEAR);

  this->gBuffer.setReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
  glBlitFramebuffer(0, 0, width, height,
                    HalfWidth, HalfHeight, width, height,
                    GL_COLOR_BUFFER_BIT, GL_LINEAR);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER,
                    0);

  glBindFramebuffer(GL_READ_FRAMEBUFFER,
                    this->gBuffer.getPointerFBO());
  glReadBuffer(GL_COLOR_ATTACHMENT0 + 2);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER,
                    0);
  glBlitFramebuffer(0, 0, width, height,
                    HalfHeight, height, HalfWidth, height,
                    GL_COLOR_BUFFER_BIT, GL_LINEAR);

  /*
  this->gBuffer.setReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
  glBlitNamedFramebuffer(this->gBuffer.getPointerFBO(), 0,
                         0, 0, width, height,
                         HalfWidth, HalfHeight, width, height,
                         GL_COLOR_BUFFER_BIT, GL_LINEAR);
                         */
}

void DeferredShader::loadShaders(const std::string& path_geometry_vert_shader,
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
    readShader(path_light_frag_shader);

  GLuint light_frag_shader = compileShader(GL_FRAGMENT_SHADER,
                                           light_frag_shader_buffer.str());
  this->light_pass_sp = createProgram(light_vert_shader, light_frag_shader);
}

}
}
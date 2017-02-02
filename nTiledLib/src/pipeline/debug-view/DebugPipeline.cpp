#include "pipeline\debug-view\DebugPipeline.h"

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
#include "pipeline\pipeline-util\ConstructQuad.h"
#include "pipeline\shader-util\LoadShaders.h"

// ----------------------------------------------------------------------------
// Defines
// ----------------------------------------------------------------------------
#define VERT_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/implementation_new/nTiled/nTiled/src/pipeline/debug-view/shaders-glsl/view-pipeline/view_pipeline.vert")
#define FRAG_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/implementation_new/nTiled/nTiled/src/pipeline/debug-view/shaders-glsl/view-pipeline/view_pipeline.frag")


namespace nTiled {
namespace pipeline {

DebugPipeline::DebugPipeline(Pipeline* frame_pipeline,
                             state::State& state) :
    Pipeline(state),
    frame_pipeline(frame_pipeline) { 
  // initialise framebuffer object
  // -----------------------------
  // Create FBO
  glGenFramebuffers(1, &(this->p_fbo));
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->p_fbo);

  // Create Texture pass result
  glGenTextures(1, &(this->p_pass_result));

  // Set up created texture
  glBindTexture(GL_TEXTURE_2D, this->p_pass_result);
  glTexImage2D(GL_TEXTURE_2D,               // target
               0,                           // level (mipmap level)
               GL_RGB32F,                   // internal format
               this->state.view.viewport.x,
               this->state.view.viewport.y, // dimensions
               0,                           // border
               GL_RGB,                      // format
               GL_UNSIGNED_SHORT_5_6_5,     // type
               NULL                         // data
               );

  // set 1 to 1 mapping so no interpolation between pixels occurs
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // Attach texture the framebuffer object
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,       // target
                         GL_COLOR_ATTACHMENT0,      // attachment
                         GL_TEXTURE_2D,             // textarget
                         this->p_pass_result,       // texture
                         0                          // level (mipmap level)
                         );

  // draw bufers
  glDrawBuffer(GL_COLOR_ATTACHMENT0);

  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE) {
    throw std::runtime_error(std::string("FB error, status: 0x%x\n", status));
  }

  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

  // Initialise full screen quad
  // ---------------------------
  this->display_quad = constructQuad();

  // Initialise debug shaders
  // ------------------------
  for (DebugShaderId id : this->state.shading.debug_shader_ids) {

  }

  // Initialise shader program
  // -------------------------
  std::stringstream vert_shader_buffer = readShader(VERT_PATH);
  GLuint vert_shader = compileShader(GL_VERTEX_SHADER,
                                     vert_shader_buffer.str());

  std::stringstream frag_shader_buffer = readShader(FRAG_PATH);
  GLuint frag_shader = compileShader(GL_FRAGMENT_SHADER,
                                     frag_shader_buffer.str());

  this->display_pipeline_sp = createProgram(vert_shader, 
                                            frag_shader);

  // Set static uniform values
  // -------------------------
  // TODO: fix this into some nice z_value placement
  GLfloat z_value = 1.0f;
  GLint p_z_value = glGetUniformLocation(this->display_pipeline_sp,
                                         "z_value");

  glm::vec4 viewport = glm::vec4(0.0f, 0.0f,
                                 this->state.view.viewport);
  GLint p_viewport = glGetUniformLocation(this->display_pipeline_sp,
                                          "viewport");

  GLint p_result_tex = glGetUniformLocation(this->display_pipeline_sp,
                                            "result_tex");

  glUseProgram(this->display_pipeline_sp);
  glUniform1f(p_z_value, z_value);                         // z_value
  glUniform4fv(p_viewport, 1, glm::value_ptr(viewport));   // viewport
  glUniform1i(p_result_tex, GL_TEXTURE0);                  // result_tex
  glUseProgram(0);

  // set output buffer of render pipeline
  this->frame_pipeline->setOutputBuffer(this->p_fbo);
}

DebugPipeline::~DebugPipeline() {
  for (DebugShader* p_shader : this->debug_shaders) {
    delete p_shader;
  }
}

void DebugPipeline::render() {
  // render the pipeline image
  // -------------------------
  // activate frame buffer object for writing
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->p_fbo);

  // clear old contents
  glClear(GL_COLOR_BUFFER_BIT);
  
  this->frame_pipeline->render();

  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

  // activate texture for render shader
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, this->p_pass_result);

  glUseProgram(this->display_pipeline_sp);
  glBindVertexArray(this->display_quad->vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
               this->display_quad->element_buffer);
  glDrawElements(GL_TRIANGLES,
                 this->display_quad->n_elements,
                 GL_UNSIGNED_SHORT, 0);
  glUseProgram(0);

  // render overlay shaders
  for (DebugShader* p_shader : this->debug_shaders) {
    p_shader->render();
  }
}

} // pipeline 
} // nTiled
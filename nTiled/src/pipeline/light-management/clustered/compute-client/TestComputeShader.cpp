#include "pipeline\light-management\clustered\compute-client\TestComputeShader.h"
#include "pipeline\shader-util\LoadShaders.h"

#include <fstream>
#include <sstream>
#include <iostream>

#define KEY_COMPUTE_SHADER_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/implementation_new/nTiled/nTiled/src/pipeline/light-management/clustered/compute-glsl/test_compute.glsl")

namespace nTiled {
namespace pipeline {
namespace clustered {

TestComputeShader::TestComputeShader() {
  this->loadComputeShaders();
  this->createFBO();

  // Link created output texture with program
  // ----------------------------------------
  GLuint p_result = glGetUniformLocation(this->compute_shader,
                                         "result");
  glUseProgram(this->compute_shader);
  glUniform1i(p_result, 0);
  glUseProgram(0);
}

void TestComputeShader::execute() {
  // execute program
  // ---------------
  glUseProgram(this->compute_shader);

  //glDrawBuffer(GL_NONE);

  // Bind and clear FBO
  /*
  glBindFramebuffer(GL_FRAMEBUFFER, this->p_fbo);
  glClear(GL_COLOR_BUFFER_BIT);
  glMemoryBarrier(GL_ALL_BARRIER_BITS);
  */

  // Bind the output texture
  glBindImageTexture(0, 
                     this->p_k_tex, 
                     0, 
                     GL_FALSE,
                     0, 
                     GL_WRITE_ONLY,
                     GL_RGBA8);

  glDispatchCompute(1, 1, 1);
  glMemoryBarrier(GL_ALL_BARRIER_BITS);

  glBindImageTexture(0,
                     0,
                     0,
                     GL_FALSE,
                     0,
                     GL_READ_WRITE,
                     GL_RGBA8);

  // read values
  GLubyte value[] = { 1, 1, 1, 1 };
  glBindTexture(GL_TEXTURE_2D, this->p_k_tex);
  glGetTexImage(GL_TEXTURE_2D,
                0,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                value);
  glBindTexture(GL_TEXTURE_2D, 0);
  //glReadPixels(0, 0, 1, 1, GL_RGBA, GL_FLOAT, value);
  // Unbind framebuffer
  //glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void TestComputeShader::loadComputeShaders() {
  std::stringstream compute_program_buffer =
    readShader(KEY_COMPUTE_SHADER_PATH);
  this->compute_shader = createComputeProgram(compute_program_buffer.str());
}

void TestComputeShader::createFBO() {
  // Generate and bind FBO
  // ---------------------
  /*
  glGenFramebuffers(1, &(this->p_fbo));
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->p_fbo);
  */

  // Construct output texture
  // ------------------------
  glGenTextures(1, &(this->p_k_tex));
  glBindTexture(GL_TEXTURE_2D, this->p_k_tex);
  
  // set texture attributes
  // ----------------------
  // set texture wrapping
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
 
  // one to one mapping
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 1);

  // Specify texture
  // ---------------
  /*
  glTexImage2D(GL_TEXTURE_2D,      // target
               0,                  // mipmap level
               GL_RGBA8,           // internal format
               1, 1,               // Texture dimensions
               0,                  // border
               GL_RGBA,            // format
               GL_UNSIGNED_BYTE,   // type
               NULL                // initial data
               );
               */
  glTexStorage2D(GL_TEXTURE_2D,
                 GLint(1),
                 GL_RGBA8,
                 1, 1);

  // Attach Texture to the created framebuffer
  // -----------------------------------------
  /*
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
                         GL_COLOR_ATTACHMENT0,
                         GL_TEXTURE_2D,
                         this->p_k_tex,
                         0);
                         */

  // Verify FBO configuration
  // ------------------------
  /*
  GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE) {
    throw std::runtime_error(std::string("Frame buffer error, status: 0x%x\n", status));
  }
  */

  // Unbind FBO
  // ----------
  /*
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  */
  glBindTexture(GL_TEXTURE_2D, 0);
}

}
}
}

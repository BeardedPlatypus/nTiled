#include "pipeline\forward\DepthBuffer.h"

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <iostream>

namespace nTiled {
namespace pipeline {

DepthBuffer::DepthBuffer(unsigned int width, unsigned height) : width(width), 
                                                                height(height) {
  // Create FBO
  // --------------------------------------------------------------------------
  glGenFramebuffers(1, &(this->p_fbo));
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->p_fbo);

  // Create colour output texture buffer
  // --------------------------------------------------------------------------
  glGenTextures(1, &(this->p_colour_texture));

  glBindTexture(GL_TEXTURE_2D, this->p_colour_texture);
  glTexImage2D(GL_TEXTURE_2D,
               0,
               GL_RGB32F,
               this->width, this->height,
               0,
               GL_RGB,
               GL_UNSIGNED_SHORT_5_6_5,
               NULL
               );
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // Attach texture the framebuffer object
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,     // target
                         GL_COLOR_ATTACHMENT0,    // attachment
                         GL_TEXTURE_2D,           // textarget
                         this->p_colour_texture,  // texture
                         0                        // level (mipmap level)
                         );

  // Create depth texture buffer
  // --------------------------------------------------------------------------
  glGenTextures(1, &(this->p_depth_texture));

  glBindTexture(GL_TEXTURE_2D, this->p_depth_texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D,
               0,
               GL_DEPTH_COMPONENT32F,
               this->width, this->height,
               0,
               GL_DEPTH_COMPONENT,
               GL_FLOAT,
               NULL
               );
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
                         GL_DEPTH_ATTACHMENT,
                         GL_TEXTURE_2D,
                         this->p_depth_texture,
                         0
                         );

  //glDrawBuffer(GL_NONE);
  GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0 };
  glDrawBuffers(1, draw_buffers);

  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE) {
    throw std::runtime_error(std::string("FB error, status: 0x%x\n", status));
  }

  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}


void DepthBuffer::bindForWriting() {
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->p_fbo);
}

void DepthBuffer::unbindForWriting() {
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}


void DepthBuffer::bindForReading() {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, this->p_colour_texture);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, this->p_depth_texture);
}


} // pipeline
} // nTiled

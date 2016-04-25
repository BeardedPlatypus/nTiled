#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glad\glad.h>

namespace nTiled {
namespace pipeline {

/*!
 GBuffer class holds all the functionality to allow the geometry pass to write 
 different components to texture memory. 
 As well as provide the methods to load these generated textures to memory 
 for the light pass to act upon.
 */
class GBuffer {
public:
  /*!
   Naming of the different GBuffer texture
   */
  enum GBUFFER_TEXTURE_TYPE {
    GBUFFER_TEXTURE_TYPE_DIFFUSE,
    GBUFFER_TEXTURE_TYPE_NORMAL,
    GBUFFER_NUM_TEXTURES
  };

  /*!
   Construct a new GBuffer with the given dimensions

   Param: 
       width  (unsigned int): the width of the textures of this new GBuffer
       height (unsigned int): the height of the texture of this new GBuffer
   */
  GBuffer(unsigned int width, unsigned int height);

  // Render related functions
  void bindForWriting();
  void unbindForWriting();

  void bindForReading();
  void setReadBuffer(GBUFFER_TEXTURE_TYPE TextureType);

  GLuint getPointerFBO() { return this->p_fbo; }
  GLuint getPointerDepthTexture() { return this->p_depth_texture; }

private:
  // GBuffer attributes
  // --------------------------------------------------------------------------
  /*! Width of the textures of this GBuffer */
  unsigned int width;
  /*! Height of the textures of this GBuffer*/
  unsigned int height;

  //  openGL pointers
  // --------------------------------------------------------------------------
  /*! Framebuffer object pointer */
  GLuint p_fbo;
  /*! Array of pointers to the textures of this GBuffer */
  GLuint p_textures[GBUFFER_NUM_TEXTURES];
  /*! Pointer to the depth texture of this GBuffer */
  GLuint p_depth_texture;
};

} // pipeline
} // nTiled

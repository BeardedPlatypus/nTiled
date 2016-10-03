/*! @file GBuffer.h
 *  @brief GBuffer.h contains the definition of the GBuffer used in nTiled
 *         for deferred shading.
 */
#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glad\glad.h>

namespace nTiled {
namespace pipeline {

/*! @brief GBuffer class holds all the functionality to allow the geometry pass 
 *         to write different components to texture memory. 
 *         As well as provide the methods to load these generated textures to 
 *         memory for the light pass to act upon.
 */
class GBuffer {
public:
  /*! @brief Naming of the different GBuffer texture */
  enum GBUFFER_TEXTURE_TYPE {
    GBUFFER_TEXTURE_TYPE_DIFFUSE,
    GBUFFER_TEXTURE_TYPE_NORMAL,
    GBUFFER_NUM_TEXTURES
  };

  /*! @brief Construct a new GBuffer with the given dimensions
   *
   * @param width  The width of the textures of this new GBuffer
   * @param height The height of the texture of this new GBuffer
   */
  GBuffer(unsigned int width, unsigned int height);

  // Render related functions
  /*! @brief Bind this GBuffer such that values will be written to it.
   */
  void bindForWriting();

  /*! @brief Unbind this GBuffer such that values will not be written
   *         to it.
   */
  void unbindForWriting();

  /*! @brief Bind this GBuffer such that values can be read from it. */
  void bindForReading();
  
  /*! @brief Set the buffer which is read to TextureType.
   * 
   * @param TextureType The type of texture which can be read.
   */
  void setReadBuffer(GBUFFER_TEXTURE_TYPE TextureType);

  /*! @brief Get the openGL pointer to the Frame Buffer Object (FBO) 
   * 
   * @return openGL pointer to the FBO.
   */
  GLuint getPointerFBO() { return this->p_fbo; }

  /*! @brief Get the openGL pointer to the depth texture. 
   *
   * @return openGL pointer to the depth texture.
   */
  GLuint getPointerDepthTexture() { return this->p_depth_texture; }

private:
  // GBuffer attributes
  // --------------------------------------------------------------------------
  /*! @brief Width of the textures of this GBuffer */
  unsigned int width;
  /*! @brief Height of the textures of this GBuffer*/
  unsigned int height;

  //  openGL pointers
  // --------------------------------------------------------------------------
  /*! @brief Framebuffer object pointer */
  GLuint p_fbo;
  /*! @brief Array of pointers to the textures of this GBuffer */
  GLuint p_textures[GBUFFER_NUM_TEXTURES];
  /*! @brief Pointer to the depth texture of this GBuffer */
  GLuint p_depth_texture;
};

} // pipeline
} // nTiled

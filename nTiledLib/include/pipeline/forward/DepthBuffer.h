/*! @file DepthBuffer
 *  @brief DepthBuffer.h contains the definition of the DepthBuffer used in 
 *         nTiled for Forward Rendering.
 */
#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glad\glad.h>

namespace nTiled {
namespace pipeline {

/*! @brief DepthBuffer class holds all the functionality to allow 
 *         to render a single depth pass and store it in video memory.
 */
class DepthBuffer {
public:
  /*! Construct a new DepthBuffer with the given parameters
   *
   * @param width  The width of the textures of this new DepthBuffer
   * @param height The height of the texture of this new DepthBuffer
   */
  DepthBuffer(unsigned int width, unsigned height);

  /*! @brief Bind this DepthBuffer such that values will be written to it.
   */
  void bindForWriting();

  /*! @brief Unbind this DepthBuffer such that values will not be written
   *         to it.
   */
  void unbindForWriting();

  /*! @brief Bind this DepthBuffer such that values can be read from it. */
  void bindForReading();

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
  /*! @brief Width of the textures of this DepthBuffer */
  const unsigned int width;
  /*! @brief Height of the textures of this DepthBuffer*/
  const unsigned int height;

  /*! @brief Framebuffer object pointer */
  GLuint p_fbo;
  /*! @brief Pointer to the depth texture of this DepthBuffer. */
  GLuint p_depth_texture;
  /*! @brief Poiter to the colour texture of this DepthBuffer. */
  GLuint p_colour_texture;
};

}
}

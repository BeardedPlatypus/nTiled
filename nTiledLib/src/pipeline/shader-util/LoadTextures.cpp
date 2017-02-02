#include "pipeline\shader-util\LoadTextures.h"

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include "math\util.h"

namespace nTiled {
namespace pipeline {

GLuint loadTexturePNG(const std::string& file_path) {
  // Load File
  std::vector<unsigned char> buffer;
  lodepng::load_file(buffer, file_path);


  // Decode file
  lodepng::State state;
  std::vector<unsigned char> image;
  unsigned width, height;
  unsigned error = lodepng::decode(image,
                                   width,
                                   height,
                                   state,
                                   buffer);

  // Parse error
  if (error != 0) {
    throw std::runtime_error(std::string("Can't read png in file: " + file_path +
                                         "\nError: " + lodepng_error_text(error)));
  }
  const LodePNGInfo& info = state.info_png;

  // Obtain png format information
  // assumption we don't care about bit depth, might need to check this later on
  // assumption every image has rgba channels
  GLint format = GL_RGBA;

  // Get pow2 width and height because openGL
  size_t width_pow2 = math::getNextPow2(width);
  size_t height_pow2 = math::getNextPow2(height);

  // Construct base image
  std::vector<unsigned char> image_pow2(width_pow2 * height_pow2 * 4);
  for (size_t y = 0; y < height; y++) {
    for (size_t x = 0; x < width; x++) {
      for (size_t c = 0; c < 4; c++) {
        image_pow2[4 * width_pow2 * y + 4 * x + c] =
          image[4 * width_pow2 * (height - 1 - y) +  // the y position 
          4 * x +               // the x position
          c];                   // channel
      }
    }
  }

  // Construct openGL texture
  GLuint p_texture;
  glGenTextures(1, &p_texture);
  glBindTexture(GL_TEXTURE_2D, p_texture);
  glTexImage2D(GL_TEXTURE_2D,        // target
               0,                    // level
               format,               // internalFormat
               width_pow2,           // width
               height_pow2,          // height
               0,                    // border
               format,               // format
               GL_UNSIGNED_BYTE,     // type
               &image_pow2[0]);      // data
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  return p_texture;
}

} // pipeline
} // nTiled 
/*! @file LoadTextures.h 
 *  @brief LoadTextures.h contains all functions related to the 
 *         loading of textures
 */
#pragma once

// ----------------------------------------------------------------------------
// Libraries
// ----------------------------------------------------------------------------
#include <string>
#include <glad\glad.h>

#include "lodepng.h"

namespace nTiled {
namespace pipeline {

/* @brief Load the png image at the given file location into video memory.
 * 
 * @param file_name Path to the png image to be loaded into memory.
 *
 * @return openGL pointer to the video memory location of the newly loaded
 *         texture.
 */
GLuint loadTexturePNG(const std::string& file_name);

} // pipeline
} // nTiled


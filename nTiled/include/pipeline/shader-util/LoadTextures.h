#pragma once

#include <string>
#include <glad\glad.h>

#include "lodepng.h"

namespace nTiled {
namespace pipeline {

GLuint loadTexturePNG(const std::string& file_name);

} // pipeline
} // nTiled


#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glad\glad.h>
#include <glm\glm.hpp>


namespace nTiled {
namespace pipeline {

struct PipelineLight {
  glm::vec4 positionCoordinates;
  glm::vec3 intensity;
  GLfloat radius;
  GLint is_emitting;
  GLint _pad[3]; // Add 3 padding to align PipeLineLightData for gpu
};

} // pipeline
} // nTiled
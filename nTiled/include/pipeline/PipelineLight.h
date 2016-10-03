/*! @file PipelineLight.h 
 *  @brief PipelineLight.h contains the definition of the PipelineLight 
 *         struct.
 */
#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glad\glad.h>
#include <glm\glm.hpp>


namespace nTiled {
namespace pipeline {

/*! @brief The PipelineLight struct contains all information for a shader to
 *         to render that light. It is set up so it can be loaded into
 *         openGL directly.
 */
struct PipelineLight {
  /*! @brief The position coordinates of this PipelineLight. */
  glm::vec4 positionCoordinates;
  /*! @brief The intensity in rgb values between 0 and 1. */
  glm::vec3 intensity;
  /*! @brief The influence radius of this PipelineLight. */
  GLfloat radius;
  /*! @brief Whether this light is emitting light or not. */
  GLint is_emitting;
  GLint _pad[3]; // Add 3 padding to align PipeLineLightData for gpu
};

} // pipeline
} // nTiled
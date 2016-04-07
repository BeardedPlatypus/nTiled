#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glm\glm.hpp>

namespace nTiled {
namespace camera {

struct CameraData {
  // constructor
  CameraData(glm::mat4 lookAt,
             glm::mat4 perspective_matrix,
             glm::vec2 depthrange);

  // member variables
  glm::mat4 lookAt;
  glm::mat4 perspective_matrix;
  glm::vec2 depthrange;
};

} // camera
} // nTiled
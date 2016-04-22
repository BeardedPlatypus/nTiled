#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glm\glm.hpp>

namespace nTiled {
namespace camera {

struct CameraConstructionData {
  CameraConstructionData(glm::vec3 camera_eye,
                         glm::vec3 camera_center,
                         glm::vec3 camera_up,
                         float fovy,
                         float aspect,
                         float z_near,
                         float z_far);

  // look at
  glm::vec3 camera_eye;
  glm::vec3 camera_center;
  glm::vec3 camera_up;

  // perspective
  float fovy;
  float aspect;

  // distance
  float z_near;
  float z_far;
};

}
}

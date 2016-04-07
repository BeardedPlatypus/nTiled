#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glm\glm.hpp>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "camera\CameraControl.h"
#include "camera\CameraData.h"

namespace nTiled {
namespace camera {

class Camera {
 public:
  // Constructor
  Camera(CameraControl* control,
         glm::vec3 camera_eye,
         glm::vec3 camera_center,
         glm::vec3 camera_up,
         float fovy,
         float aspect,
         float z_near,
         float z_far);
  Camera();

  // Access Camera Data
  glm::mat4 getLookAt() const;
  glm::mat4 getPerspectiveMatrix() const;
  glm::vec2 getDepthrange() const;

  // Update Camera data based on the io and the set CameraControl
  void update(const ImGuiIO& io);
  void toFocus(const ImGuiIO& io);

 private:
  CameraControl* control;
  CameraData data;

  bool is_active;
};

} // camera
} // nTiled
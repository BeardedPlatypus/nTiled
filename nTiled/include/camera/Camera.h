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
#include "camera\CameraConstructionData.h"

namespace nTiled {
namespace camera {

class Camera {
 public:
  // Constructor
  Camera(CameraControl* control,
         CameraConstructionData construction_data);
  Camera();

  // Access Camera Data
  glm::mat4 getLookAt() const;
  glm::mat4 getPerspectiveMatrix() const;
  glm::vec2 getDepthrange() const;

  // Access Camera construction Data
  float getFoV() const;

  // Update Camera data based on the io and the set CameraControl
  void update(const ImGuiIO& io);
  void toFocus(const ImGuiIO& io);

 private:
  CameraControl* control;
  CameraConstructionData construction_data;

  CameraData data;
  bool is_active;
};

} // camera
} // nTiled
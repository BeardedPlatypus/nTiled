#include "camera\CameraControl.h"

// ----------------------------------------------------------------------------
//  System Libraries
// ----------------------------------------------------------------------------
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace nTiled {
namespace camera {

// ----------------------------------------------------------------------------
//  TurnTableControl
// ----------------------------------------------------------------------------
//  Constructors
TurnTableCameraControl::TurnTableCameraControl(double sensitivity) :
  sensitivity(sensitivity),
  pos_prev(ImVec2()) {
}

TurnTableCameraControl::TurnTableCameraControl() :
  TurnTableCameraControl(320.0) {
}

// ----------------------------------------------------------------------------
//  Methods
void TurnTableCameraControl::update(const ImGuiIO& io,
                                    CameraData &data) {
  // y values map to camera space x
  // x values map to world space y
  ImVec2 pos_cur = io.MousePos;
  // camera x-axis rotations
  double dif_y = (pos_cur.y - this->pos_prev.y);
  float angle_x = (float)dif_y / this->sensitivity;

  glm::mat4 lookAt = data.lookAt;
  glm::vec3 camera_x_axis = glm::vec3(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f) * lookAt);
  lookAt = glm::rotate(lookAt, angle_x, camera_x_axis);

  // world y-axis rotations
  double dif_x = (pos_cur.x - this->pos_prev.x);
  float angle_y = (float)dif_x / this->sensitivity;
  lookAt = glm::rotate(lookAt, angle_y, glm::vec3(0.0f, 1.0f, 0.0f));

  // update variables
  data.lookAt = lookAt;
  this->pos_prev = pos_cur;
}

void TurnTableCameraControl::activate(const ImGuiIO& io) {
  this->pos_prev = io.MousePos;
}


// ----------------------------------------------------------------------------
//  AutomaticRotateCameraControl
// ----------------------------------------------------------------------------
// Constructors
AutomaticRotateCameraControl::AutomaticRotateCameraControl(double rotation_speed) :
  rotation_speed(rotation_speed) {
} // rotation speed in degrees per second

AutomaticRotateCameraControl::AutomaticRotateCameraControl() :
  AutomaticRotateCameraControl(60.0) {
}

// ----------------------------------------------------------------------------
//  Methods
void AutomaticRotateCameraControl::update(const ImGuiIO& io,
                                          CameraData &data) {
  // look up time between last two calls
  // assume rotation along the z-axis with camera speed time
  // multiply old camera data with the matrix based on the time
}

void AutomaticRotateCameraControl::activate(const ImGuiIO& io) {
}

} // camera
} // pipeline

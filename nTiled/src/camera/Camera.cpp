#include "camera\Camera.h"

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glm/gtc/matrix_transform.hpp>

// ----------------------------------------------------------------------------
//  Camera
// ----------------------------------------------------------------------------
//  Constructors
namespace nTiled {
namespace camera {

Camera::Camera(CameraControl* control,
               glm::vec3 camera_eye,
               glm::vec3 camera_center,
               glm::vec3 camera_up,
               float fovy,
               float aspect,
               float z_near,
               float z_far) :
  control(new TurnTableCameraControl()),
  data(CameraData(glm::lookAt(camera_eye,
                              camera_center,
                              camera_up),
                  glm::perspective(fovy,
                                   aspect,
                                   z_near,
                                   z_far),
                  glm::vec2(z_near, z_far))) {
}

Camera::Camera() : Camera::Camera(new TurnTableCameraControl(),
                                  glm::vec3(-1.0, 0.0, 0.0),
                                  glm::vec3(0.0),
                                  glm::vec3(0.0, 1.0, 0.0),
                                  45.0,
                                  1.0,
                                  1.0,
                                  50.0) {
}

// ----------------------------------------------------------------------------
glm::mat4 Camera::getLookAt() const {
  return this->data.lookAt;
}

glm::mat4 Camera::getPerspectiveMatrix() const {
  return this->data.perspective_matrix;
}

glm::vec2 Camera::getDepthrange() const {
  return this->data.depthrange;
}

void Camera::update(const ImGuiIO& io) {
  this->control->update(io, this->data);
}

void Camera::toFocus(const ImGuiIO& io) {
  this->control->activate(io);
}

} // camera
} // nTiled
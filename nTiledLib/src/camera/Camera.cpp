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
               CameraConstructionData construction_data) :
  control(control),
  construction_data(construction_data),
  data(CameraData(glm::lookAt(construction_data.camera_eye,
                              construction_data.camera_center,
                              construction_data.camera_up),
                  glm::perspective(construction_data.fovy,
                                   construction_data.aspect,
                                   construction_data.z_near,
                                   construction_data.z_far),
                  glm::vec2(construction_data.z_near, 
                            construction_data.z_far))) {
}

Camera::Camera() : Camera::Camera(new TurnTableCameraControl(),
                                  CameraConstructionData(glm::vec3(-1.0, 0.0, 0.0),
                                                         glm::vec3(0.0),
                                                         glm::vec3(0.0, 1.0, 0.0),
                                                         45.0,
                                                         1.0,
                                                         1.0,
                                                         50.0)) {
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

float Camera::getFoV() const {
  return this->construction_data.fovy;
}

void Camera::update(const ImGuiIO& io) {
  this->control->update(io, this->data);
}

void Camera::toFocus(const ImGuiIO& io) {
  this->control->activate(io);
}

} // camera
} // nTiled
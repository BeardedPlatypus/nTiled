#include "state\StateView.h"

namespace nTiled {
namespace state {

// ----------------------------------------------------------------------------
// View
View::View(camera::Camera camera,
           camera::CameraControl* p_camera_control,
           glm::uvec2 viewport,
           ViewOutput* output) :
  camera(camera),
  camera_control(p_camera_control),
  viewport(viewport), 
  output(output) {
}

View::~View() {
  // Delete camera control
  delete this->camera_control;
  delete this->output;
}

// ----------------------------------------------------------------------------
// ViewOutput
ViewOutput::ViewOutput(const std::string& image_base_path,
                       const unsigned int frame_start,
                       const unsigned int frame_end) :
  type(OutputType::Memory),
  image_base_path(image_base_path),
  frame_start(frame_start),
  frame_end(frame_end) { 
}

ViewOutput::ViewOutput() :
  type(OutputType::Display),
  image_base_path(""),
  frame_start(-1),
  frame_end(-1) {
}


} // state
} // nTiled
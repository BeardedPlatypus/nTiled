#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "camera\Camera.h"

namespace nTiled { 
namespace state {

enum class OutputType {
  Display,
  Memory,
};


struct ViewOutput {
  // Constructors
  ViewOutput(const std::string& image_base_path,
             const unsigned int frame_start,
             const unsigned int frame_end);
  ViewOutput();

  // Member elements
  OutputType type;
  const std::string image_base_path;
  const unsigned int frame_start;
  const unsigned int frame_end;
};


/*!
 View contains all attributes related to the view of the State
 */
struct View {
  View(camera::Camera camera,
       camera::CameraControl* camera_control,
       glm::uvec2 viewport,
       ViewOutput* output);
	// Destructor
	~View();

	// Member elements
	camera::Camera camera;
	camera::CameraControl* camera_control;
	glm::uvec2 viewport;

  const ViewOutput* output;
};


} // state
} // nTiled
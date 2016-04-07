#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "camera\Camera.h"

namespace nTiled { 
namespace state {

/*!
 View contains all attributes related to the view of the State
 */
struct View {
	// Constructor
	View(camera::Camera camera,
		   camera::CameraControl* camera_control,
		   glm::uvec2 viewport);
	// Destructor
	~View();

	// Member elements
	camera::Camera camera;
	camera::CameraControl* camera_control;
	glm::uvec2 viewport;
};

} // state
} // nTiled
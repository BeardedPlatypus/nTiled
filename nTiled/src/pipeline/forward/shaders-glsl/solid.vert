#version 440

// input buffers
// ----------------------------------------------------------------------------
layout (location=0) in vec4 vertex_position;

// Variable definitions
// ----------------------------------------------------------------------------
// camera definition
uniform mat4 camera_to_clip;
uniform mat4 model_to_camera;

// ----------------------------------------------------------------------------
//  Main
// ----------------------------------------------------------------------------
void main() {
	// calculate camera coordinates
    vec4 vertex_camera_coordinates = model_to_camera * vertex_position;
	// calculate vertex clipspace coordinates
	gl_Position = camera_to_clip * vertex_camera_coordinates;
}

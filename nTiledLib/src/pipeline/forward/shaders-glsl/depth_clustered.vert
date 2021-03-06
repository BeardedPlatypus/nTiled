#version 440

// Vertex Input Buffers
// -----------------------------------------------------------------------------
layout (location=0) in vec4 vertex_position;
layout (location=1) in vec3 vertex_normal;
layout (location=2) in vec3 vertex_uv;

// Variable Definitions
// -----------------------------------------------------------------------------
// Camera Definition
uniform mat4 camera_to_clip;
uniform mat4 model_to_camera;

// -----------------------------------------------------------------------------
//  Main
// -----------------------------------------------------------------------------
void main() {
    // Calculate camera position
    vec4 vertex_camera_position = model_to_camera * vertex_position;
    gl_Position = camera_to_clip * vertex_camera_position;
}

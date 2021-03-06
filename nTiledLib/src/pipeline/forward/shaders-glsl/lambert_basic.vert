#version 440

// Vertex Input Buffers
// -----------------------------------------------------------------------------
layout (location=0) in vec4 vertex_position;
layout (location=1) in vec3 vertex_normal;
layout (location=2) in vec3 vertex_uv;

// Vertex Output Buffers
// -----------------------------------------------------------------------------
out vec4 fragment_position;
out vec3 fragment_normal;

// Variable Definitions
// -----------------------------------------------------------------------------
// Camera Definition
uniform mat4 camera_to_clip;
uniform mat4 model_to_camera;
uniform mat4 inv_transpose_model_to_camera;

// -----------------------------------------------------------------------------
//  Main
// -----------------------------------------------------------------------------
void main() {
    // Calculate camera position
    vec4 vertex_camera_position = model_to_camera * vertex_position;
    gl_Position = camera_to_clip * vertex_camera_position;

    // Pass values to fragment shader
    fragment_position = vertex_camera_position;
    fragment_normal = (inv_transpose_model_to_camera *
                       vec4(vertex_normal, 0.0f)).xyz;
}
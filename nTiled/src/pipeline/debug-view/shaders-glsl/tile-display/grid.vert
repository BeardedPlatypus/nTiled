#version 440

// Input buffers
// ----------------------------------------------------------------------------
layout (location=0) in vec3 pos;

// Main
// ----------------------------------------------------------------------------
void main() {
    gl_Position = vec4(pos, 1.0);
}

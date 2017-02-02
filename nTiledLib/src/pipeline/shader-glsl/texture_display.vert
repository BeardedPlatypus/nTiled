#version 440

// Vertex Input Buffers
// -----------------------------------------------------------------------------
layout (location = 0) in vec4 position;

// -----------------------------------------------------------------------------
//  Main
// -----------------------------------------------------------------------------
void main() {
    gl_Position = vec4(position.xyz, 1.0f);
}
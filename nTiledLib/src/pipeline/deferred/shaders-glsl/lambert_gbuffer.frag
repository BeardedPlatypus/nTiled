#version 440

// Fragment Input Buffers
// -----------------------------------------------------------------------------
in vec3 fragment_normal;

// Fragment Output Buffers
// -----------------------------------------------------------------------------
layout (location=0) out vec3 diffuse_out;
layout (location=1) out vec3 normal_out;

// -----------------------------------------------------------------------------
// Main
// -----------------------------------------------------------------------------
void main() {
    // For basic lambert we assume the diffuse colour to be white
    diffuse_out = vec3(1.0f);
    normal_out = normalize(fragment_normal);
}
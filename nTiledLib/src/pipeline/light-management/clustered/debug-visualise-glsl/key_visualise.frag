#version 440

// output
// ----------------------------------------------------------------------------
out vec4 fragment_colour;

// Variables
// ----------------------------------------------------------------------------
uniform vec2 tile_size;
uniform vec2 colour_step;
uniform vec2 viewport;

uniform float k_step;
layout(binding = 0) uniform usampler2D k_tex;

// Function Definitions
// -----------------------------------------------------------------------------
/*!
 * Compute the texture coordinates of this fragment.
 */
vec2 calcTextureCoordinates() {
    return gl_FragCoord.xy / viewport.xy;
}

// ----------------------------------------------------------------------------
// Main
// ----------------------------------------------------------------------------
void main() {
    vec2 tex_coords = calcTextureCoordinates();
    uint k = texture(k_tex, tex_coords).x;
    
    vec2 tile_of_pixel = floor((gl_FragCoord.xy - vec2(0.5)) / tile_size);
    vec2 rg = vec2(0.1) + vec2(0.9) * tile_of_pixel * colour_step;

    fragment_colour = vec4(rg, (1.0 - k * k_step), 1.0);
}

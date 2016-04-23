#version 440

// output
// ----------------------------------------------------------------------------
out vec4 fragment_colour;

// Variables
// ----------------------------------------------------------------------------
uniform vec2 tile_size;
uniform vec2 colour_step;

// ----------------------------------------------------------------------------
// Main
// ----------------------------------------------------------------------------
void main() {
    vec2 tile_of_pixel = floor((gl_FragCoord.xy - vec2(0.5)) / tile_size);
    vec2 rg = vec2(0.1) + vec2(0.9) * tile_of_pixel * colour_step;

    fragment_colour = vec4(rg, 1.0, 1.0);
}

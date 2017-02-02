#version 440

//  output
// ----------------------------------------------------------------------------
out vec4 fragment_colour;

// Variables
// ----------------------------------------------------------------------------
layout(binding = 0) uniform sampler2D result_tex;
uniform vec4 viewport;

/*!
 * Compute the texture coordinates of this fragment.
 */
vec2 calcTextureCoordinates() {
    return gl_FragCoord.xy / viewport.w;
}

// -----------------------------------------------------------------------------
// Main
// -----------------------------------------------------------------------------
void main() {
    vec2 tex_coords = calcTextureCoordinates();
    fragment_colour = vec4(texture(result_tex, tex_coords).xyz, 1.0);
    //fragment_colour = vec4(tex_coords.x, tex_coords.y, 1.0, 1.0);
}
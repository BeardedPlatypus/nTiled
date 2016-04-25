#version 440

#define TILE_SIZE_X 1
#define TILE_SIZE_Y 1

// Layout definition
// ----------------------------------------------------------------------------
//  We define the local size as a single tile.
layout( 
  local_size_x =  TILE_SIZE_X,
  local_size_y =  TILE_SIZE_Y
  ) in;

// Output
// ----------------------------------------------------------------------------
layout (binding = 0, r16ui) uniform writeonly uimage2D k_tex;

// Variable Definitions 
// ----------------------------------------------------------------------------
layout(binding = 0) uniform sampler2D depth_tex;
uniform mat4 inv_perspective_matrix;

uniform vec4 viewport;

uniform float k_inv_denominator;
uniform float near_plane_z;

// Function Definitions
// -----------------------------------------------------------------------------
/*!
 * Compute the camera coordinates of this fragment given the texture coordinates
 *
 * param:
 *     tex_coords (vec2): the texture coordinates of this fragment.
 */
vec4 getCameraCoordinates(in ivec2 screen_pos) {
    // Define the normalized device coordinates
    vec3 device;
    device.xy = (2.0f * ((screen_pos - viewport.xy) /
                          viewport.zw)) - 1.0f;
    device.z = 2.0f * texture(depth_tex, screen_pos).r - 1.0f;

    // Calculate actual coordinates
    vec4 raw_coords = inv_perspective_matrix * vec4(device, 1.0f);
    vec4 coords = raw_coords / raw_coords.w;

    return coords;
}

// ----------------------------------------------------------------------------
//  Main
// ----------------------------------------------------------------------------
void main() {
    ivec2 screen_pos = ivec2(gl_GlobalInvocationID.xy);
    vec4 camera_coords = getCameraCoordinates(screen_pos);

    uint k = uint(floor(log( camera_coords.z / near_plane_z ) * k_inv_denominator ));
    imageStore(k_tex, screen_pos, uvec4(k));
}
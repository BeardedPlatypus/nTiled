#version 440

#define TILE_SIZE_X 1
#define TILE_SIZE_Y 1

// Layout definition
//  We define the local size as a single tile.
layout( 
  local_size_x =  TILE_SIZE_X,
  local_size_y =  TILE_SIZE_Y
  ) in;

// ----------------------------------------------------------------------------
//  Input
// ----------------------------------------------------------------------------
/*! depth texture of which the keys need to be computed. */
layout(binding = 0) uniform sampler2D depth_tex;

/*! 1 / (ln ( 1 + ( (2 tan \theta ) / S_y) ) ) */
uniform float k_inv_denominator;
uniform float near_plane_z;

// ----------------------------------------------------------------------------
//  Output
// ----------------------------------------------------------------------------
// Shader Storage Buffer Object output
layout (binding = 1, r16ui) uniform writeonly uimage2D k_tex;

void main() {
    ivec2 screen_pos = ivec2(gl_GlobalInvocationID.xy);
    /*
    float k = floor(log( texture(depth_tex, screen_pos) / near_plane_z ) * k_inv_denominator ));
    imageStore(k_tex, screen_pos, k);
    */
}
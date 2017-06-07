#version 440

#define NUM_LIGHTS 3

// Fragment Output Buffers
// -----------------------------------------------------------------------------
out uint light_calculations;

// Variable Definitions
// -----------------------------------------------------------------------------
// Structs
/*! Light Struct
 *
 * position: position of this light
 * intensity: the intensity (colour) with which this light is emitting
 * radius: the radius of the light sphere of this light
 * is_emitting: whether this light is emitting or not
 */
struct Light {
    vec4 position;    // 4
    vec3 intensity;   // 3
    float radius;     // 1
    int is_emitting;  
};

/*! GeometryParam for Lambert Shading.
 *
 *  position: the position at the fragment in the same coordinate system
              as the light
 *  normal  : the normal at the fragment.
 *  colour  : the colour at the fragment.
 */
struct GeometryParam {
    vec4 position;
    vec3 normal;
    vec3 colour;
};

//  Light Management
// -----------------------------------------------------------------------------
layout (std430, binding = 0) buffer SummedClusterIndices {
    uint summed_cluster_indices[];
};

layout (std430, binding = 1) buffer LightClusterBuffer {
    uvec2 clusters[];
};

layout (std430, binding = 2) buffer LightIndexBuffer {
    uint light_indices[];
};

uniform uvec2 tile_size;
uniform uint n_tiles_x;

// -----------------------------------------------------------------------------
layout (std140) uniform LightBlock {
    Light lights[NUM_LIGHTS];
};

// -----------------------------------------------------------------------------
// MRT texture samplers
layout(binding = 0) uniform sampler2D diffuse_tex;
layout(binding = 1) uniform sampler2D normal_tex;
layout(binding = 2) uniform sampler2D depth_tex;
layout(binding = 3) uniform usampler2D k_index_tex;

// -----------------------------------------------------------------------------
// Position definition
uniform mat4 perspective_matrix;
uniform mat4 inv_perspective_matrix;

uniform vec4 viewport;
uniform vec2 depth_range;


/*!
 * Compute the camera coordinates of this fragment given the texture coordinates
 *
 * param:
 *     tex_coords (vec2): the texture coordinates of this fragment.
 */
vec4 getCameraCoordinates(in vec2 tex_coords) {
    // Define the normalized device coordinates
    vec3 device;
    device.xy = (2.0f * ((gl_FragCoord.xy - vec2(0.5f, 0.5f) - viewport.xy) /
                          viewport.zw)) - 1.0f;
    device.z = 2.0f * texture(depth_tex, tex_coords).x - 1.0f;

    // Calculate actual coordinates
    vec4 raw_coords = inv_perspective_matrix * vec4(device, 1.0f);
    vec4 coords = raw_coords / raw_coords.w;

    return coords;
}


// -----------------------------------------------------------------------------
// Main
// -----------------------------------------------------------------------------
void main() {
    vec2 tex_coords = calcTextureCoordinates();
    vec3 diffuse_colour = texture(diffuse_tex, tex_coords).xyz;
    
    if (diffuse_colour.rgb == vec3(0.0f)) {
        light_calculations = 0;
    } else {
        vec4 coords_camera_space = getCameraCoordinates(tex_coords);
        vec3 normal = normalize(texture(normal_tex, tex_coords)).xyz;

        GeometryParam param = GeometryParam(coords_camera_space,
                                            normal,
                                            diffuse_colour);

        // determine contributing lights
        vec2 screen_position = gl_FragCoord.xy - vec2(0.5f, 0.5f);
        uint tile_index = uint(floor(screen_position.x / tile_size.x) +
                               floor(screen_position.y / tile_size.y) * n_tiles_x);
        uint tile_offset = summed_cluster_indices[tile_index];
        uint k_offset = texture(k_index_tex, tex_coords).x;

        uvec2 cluster_map = clusters[tile_offset + k_offset];

        uint offset = cluster_map.x;
        uint n_lights = cluster_map.y;

        // output result
        light_calculations = n_lights;
    }   
}

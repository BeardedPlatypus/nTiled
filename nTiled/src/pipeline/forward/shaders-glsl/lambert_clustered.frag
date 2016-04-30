#version 440

#define NUM_LIGHTS 3

// Fragment Input Buffers
// -----------------------------------------------------------------------------
in vec4 fragment_position;
in vec3 fragment_normal;

// Fragment Output Buffers
// -----------------------------------------------------------------------------
out vec4 fragment_colour;

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

// -----------------------------------------------------------------------------
layout (std140) uniform LightBlock {
    Light lights[NUM_LIGHTS];
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


// Function Definitions
// -----------------------------------------------------------------------------
/*!
 * Compute lambert shading for the attenuated light.
 *
 * param:
 *     light (Light): light of this computeLight
 *     param (GeometryParam): struct containing the geometry parameters
 *                            needed for the light computation
 *                            - position: position of the fragment in the
 *                                        same coordinate system as the
 *                                        light position.
 *                            - normal: the normal (normalised) at the
 *                                      fragment.
 *                            - colour: the colour of this fragment
 */ 
vec3 computeLight(Light light,
                  GeometryParam param) {
    vec3 L = vec3(light.position - param.position);
    float d = length(L);

    if (d < light.radius) {
        vec3 light_direction = L / d;

        // compute light attenuation
        float attenuation = clamp(1.0 - ( d / light.radius ),
                                  0.0f,
                                  1.0f);
        attenuation *= attenuation;

        // compute lambert for this light
        float cos_angular_incidence = clamp(dot(param.normal, light_direction),
                                            0.0f,
                                            1.0f);
        return (param.colour *
                light.intensity *
                cos_angular_incidence * attenuation);
    } else {
        return vec3(0.0);
    }
}


// -----------------------------------------------------------------------------
// Main
// -----------------------------------------------------------------------------
void main() {
    // light accumulator
    vec3 light_acc = vec3(0.0f);

    // set geometry param
    GeometryParam param = GeometryParam(fragment_position,
                                        normalize(fragment_normal),
                                        vec3(1.0f));

    // determine contributing lights
    // determine contributing lights
    vec2 screen_position = gl_FragCoord.xy - vec2(0.5f, 0.5f);
    uint tile_index = uint(floor(screen_position.x / tile_size.x) +
                           floor(screen_position.y / tile_size.y) * n_tiles_x);
    uint tile_offset = summed_cluster_indices[tile_index];
    uint k_offset = texture(k_index_tex, tex_coords).x;

    uvec2 cluster_map = clusters[tile_offset + k_offset];

    uint offset = cluster_map.x;
    uint n_lights = cluster_map.y;

    // compute the contribution of each light
    for (uint i = offset; i < offset + n_lights; i++) {
        light_acc += computeLight(lights[light_indices[i]], param);
    }
    // output result
    fragment_colour = vec4((vec3(0.1f) + (light_acc * 0.9)), 1.0f);
}

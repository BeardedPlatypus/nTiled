#version 440

#define NUM_LIGHTS 3
#define OCTREE_DEPTH 1

// Fragment Input Buffers
// -----------------------------------------------------------------------------
in vec4 fragment_position;
in vec3 fragment_normal;

// Fragment Output Buffers
// -----------------------------------------------------------------------------
//out vec4 fragment_colour;
layout (location=0) out vec3 fragment_colour;

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
layout (std430, binding = 2) buffer LightIndexBuffer {
    uint light_indices[];
};

uniform sampler3D node_offset_tables[OCTREE_DEPTH];
uniform sampler3D node_hash_tables[OCTREE_DEPTH];

uniform sampler3D leaf_offset_tables[OCTREE_DEPTH];
uniform sampler3D leaf_hash_tables[OCTREE_DEPTH];

// octree origin in camera coordinates
uniform vec3 octree_origin;

// (1 / node_size)
uniform float node_size_base_den;

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


bvec2 obtainNodeFromHash(uvec3 coord, sampler3D offset_table, sampler3D hash_table ) {
    return texture(hash_table, coord + texture(offset_table, coord).xyz).xy
}

uvec2 obtainLeafFromHash(uvec3 coord, sampler3D offset_table, sampler3D hash_table) {
    return texture(hash_table, coord + texture(offset_table, coord).xyz).xy
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
    // Find leaf node hash.
    vec3 octree_position = ((fragment_position.xyz / fragment_position.z) - octree_origin);
    uvec3 octree_coord;
    float node_size_den = node_size_base_den;

    // traverse octree with hashing
    bvec2 partial_node = bvec2(false, false);
    uint depth = 0;

    uvec2 leaf_node = uvec2(0, 0);

    // test whether the use of depth is allowed
    for (uint depth = 0; i < OCTREE_DEPTH; depth++) {
        // traverse octree
        octree_coord = floor(octree_position * node_size);
        node_size_den *= 2;

        partial_node = obtainNodeFromHash(octree_coord, 
                                          node_offset_tables[depth], 
                                          node_hash_tables[depth]);

        // obtain results if leaf is reached
        if (partial_node.x) {
            if (partial_node.y) {
                leaf_node = obtainLeafFromHash(octree_coord, 
                                               leaf_offset_tables[depth], 
                                               leaf_hash_tables[depth]); 
            }
            break;
        }
    }

    uint offset = leaf_node.x;
    uint n_lights = leaf_node.y;

    // compute the contribution of each light
    for (uint i = offset; i < offset + n_lights; i++) {
        light_acc += computeLight(lights[light_indices[i]], param);
    }

    // output result
    fragment_colour = vec3((vec3(0.1f) + (light_acc * 0.9)));
}

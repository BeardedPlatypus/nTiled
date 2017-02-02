#version 440

#define NUM_LIGHTS 3
#define OCTREE_DEPTH 0

// Fragment Input Buffers
// -----------------------------------------------------------------------------
in vec4 fragment_position;
in vec4 fragment_octree_position;
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
layout (std430, binding = 0) buffer LightIndexBuffer {
    uint light_indices[];
};

uniform usampler3D node_offset_tables[OCTREE_DEPTH];
uniform usampler3D node_hash_tables[OCTREE_DEPTH];

uniform usampler3D leaf_offset_tables[OCTREE_DEPTH];
uniform usampler3D leaf_hash_tables[OCTREE_DEPTH];

// octree origin in camera coordinates
uniform vec3 octree_origin;

// (1.0 / node_size)
uniform float node_size_base_den;
uniform float octree_size;

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


vec3 calcTexturePosition(uvec3 coord, usampler3D sampler) {
    // textures are cube in nature, thus each dimension is of the same size
    float texel_size = 1.0 / float(textureSize(sampler, 0).x);
    return coord * texel_size;
}


uvec2 obtainNodeFromSpatialHashFunction(uvec3 coord, 
                                        usampler3D offset_table, 
                                        usampler3D hash_table) {
    uvec3 offset = texture(offset_table, 
                           calcTexturePosition(coord, offset_table)).xyz;
    uvec2 node = texture(hash_table, 
                         calcTexturePosition((coord + offset),
                                             hash_table)).xy;
    return node;
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
    vec3 octree_position = (fragment_octree_position.xyz / fragment_octree_position.w) - octree_origin;

    float node_size_den = node_size_base_den;
    uvec3 octree_coord_cur = uvec3(floor(octree_position * node_size_den));
    uvec3 octree_coord_new;

    uvec2 partial_node;
    uvec2 leaf_node = uvec2(0, 0);

    uvec3 local_node_index;
    uint node_index;

    bool is_leaf;
    bool is_non_empty;

    uint val = 0;

    // test whether the use of depth is allowed
    for (uint depth = 0; depth < OCTREE_DEPTH; depth++) {
        partial_node = obtainNodeFromSpatialHashFunction(octree_coord_cur, 
                                                         node_offset_tables[depth], 
                                                         node_hash_tables[depth]);

        node_size_den *= 0.5;
        octree_coord_new = uvec3(floor(octree_position * node_size_den));

        local_node_index = octree_coord_new - (octree_coord_cur * 2);
        node_index = local_node_index.x + local_node_index.y * 2 + local_node_index.z * 4;

        is_leaf = bool((partial_node.x & ( 1 << node_index )) >> node_index);
        is_non_empty = bool((partial_node.y & ( 1 << node_index )) >> node_index);
       
        // obtain results if leaf is reached
        if (is_leaf) {
            if (is_non_empty) {
                leaf_node = obtainNodeFromSpatialHashFunction(octree_coord_new, 
                                                              leaf_offset_tables[depth], 
                                                              leaf_hash_tables[depth]); 
            }
            break;
        } else {
            octree_coord_cur = octree_coord_new;
        }
        val = depth;
    }

    uint offset = leaf_node.x;
    uint n_lights = leaf_node.y;

    for (uint i = offset; i < offset + n_lights; i++) {
        light_acc += computeLight(lights[light_indices[i]], param);
    }

    // output result
    fragment_colour = vec3((vec3(0.1f) + (light_acc * 0.9)));

    if (!is_leaf && !is_non_empty) {
        fragment_colour = vec3(1.0f, 0.0f, 0.0f);
    }

    if (octree_position.x > octree_size || 
        octree_position.y > octree_size || 
        octree_position.z > octree_size ) {
        fragment_colour = vec3(0.0, 1.0f, 0.0f);
    }

    /*
    if (textureSize(leaf_offset_tables[0], 0).x == 5) {
        fragment_colour = vec3(0.0, 1.0f, 1.0f);
    }
    */
}

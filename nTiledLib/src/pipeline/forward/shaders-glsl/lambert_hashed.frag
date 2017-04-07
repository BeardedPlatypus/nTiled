#version 440

#define NUM_LIGHTS 3
#define OCTREE_DEPTH 0


// Fragment Input Buffers
// -----------------------------------------------------------------------------
// Position of this fragment in coordinates same as lights
in vec4 fragment_position;

// Normal of this fragment in coordinates same as lights
in vec3 fragment_normal;

// Position of this fragment in octree coordinates
in vec3 fragment_octree_position;


// Fragment Output Buffers
// -----------------------------------------------------------------------------
//out vec4 fragment_colour;
layout (location=0) out vec3 fragment_colour;


// Variable Definitions
// -----------------------------------------------------------------------------
// Structs
/*! @brief Light struct describes a single light within this pipeline.
 */
struct Light {
  /*! @brief The position of this Light in ... coordinates. */
  vec4 position;    // 4
  /*! @brief The colour / intensity of this Light. */
  vec3 intensity;   // 3
  /*! @brief The radius of this Light */
  float radius;     // 1
  /*! @brief Whether this Light is emitting light. */
  int is_emitting;  
};


/*! @brief GeometryParam struct describes the surface of a single point.
 */
struct GeometryParam {
  /*! @brief The position of the fragment of this GeometryParam in ... coordinates. */
  vec4 position;
  /*! @brief The normal of the fragment of this GeometryParam. */
  vec3 normal;
  /*! @brief The colour of the fragment of this GeometryParam. */
  vec3 colour;
};


// -----------------------------------------------------------------------------
/*! @brief Array of Lights used in this Shader. */
layout (std140) uniform LightBlock {
  Light lights[NUM_LIGHTS];
};


//  Light Management
// -----------------------------------------------------------------------------
/*! @brief Buffer of light indices used in this Shader. */
layout (std430, binding = 0) buffer LightIndexBuffer {
  uint light_indices[];
};


// octree structure linkless octree
uniform usampler3D octree_offset_tables[OCTREE_DEPTH];
uniform usampler3D octree_data_tables[OCTREE_DEPTH];


// data linkless octree
uniform usampler3D light_offset_tables[OCTREE_DEPTH];
uniform usampler3D light_data_tables[OCTREE_DEPTH];


uniform float node_size_den;
uniform float octree_width;


//uniform usampler2D test;
uniform usampler3D test;


/*! @brief Compute Lambert shading for the attenuated light and return 
 *         the colour shaded by this Light.
 *
 * @param light Light used in this Lambert shading
 * @param param Fragment description in this Lambert shading
 *
 * @returns The colour from this light add the described fragment.
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


/*! @brief Obtain the data associated at the specified coordinate from the 
 *         specified spatial hash function.
 * 
 * @param coord The coordinate to be retrieved from the spatial hash function
 * @param offset_table The offset table of the spatial hash function
 * @param hash_table The hash table of the spatial hash function
 *
 * @returns the data associated with the specified coordinate from the 
 *          the specified spatial hash function.
 */
uvec2 obtainNodeFromSpatialHashFunction(ivec3 coord, 
                                        usampler3D offset_table, 
                                        usampler3D hash_table) {
  ivec3 h_offset_mod = ivec3(mod(coord, textureSize(offset_table, 0).x));
  ivec3 offset = ivec3(texelFetch(offset_table, h_offset_mod, 0).rgb);

  ivec3 h_coord_mod = ivec3(mod((coord + offset), textureSize(hash_table, 0).x));
  uvec2 node = texelFetch(hash_table, h_coord_mod, 0).rg;
  return node;
}


/*! @brief Extract the kth bit from val 
 * 
 * @param val The value of which the bit is extracted
 * @param k_bit The index of the bit to be extracted
 *
 * @returns the kth bit of val as a boolean value.
 */
bool extractBit(uint val, uint k_bit) {
  return ((val & (1 << k_bit)) >> k_bit) == 1;
}


// -----------------------------------------------------------------------------
// Main
// -----------------------------------------------------------------------------
void main() {
  vec3 light_acc = vec3(0.0f);

  GeometryParam param = GeometryParam(fragment_position,
                                      normalize(fragment_normal),
                                      vec3(1.0f));

  // Retrieve the relevant lights.
  uvec2 light_data = uvec2(0, 0);

  if (fragment_octree_position.x >= 0.0f &&
      fragment_octree_position.y >= 0.0f &&
      fragment_octree_position.z >= 0.0f &&
      fragment_octree_position.x < octree_width &&
      fragment_octree_position.y < octree_width &&
      fragment_octree_position.z < octree_width) {

    float next_node_size_den = node_size_den;
    ivec3 octree_coord_cur;
    ivec3 octree_coord_next = ivec3(floor(fragment_octree_position * next_node_size_den));

    uvec3 index_dif;
    uint index_int;

    uvec2 octree_data;

    for (uint layer_i = 0; layer_i < OCTREE_DEPTH; ++layer_i) {
      octree_coord_cur = octree_coord_next;

      next_node_size_den *= 2;
      octree_coord_next = ivec3(floor(fragment_octree_position * next_node_size_den));
      index_dif = octree_coord_next - (octree_coord_cur * 2);
      index_int = index_dif.x + index_dif.y * 2 + index_dif.z * 4;

      octree_data = obtainNodeFromSpatialHashFunction(octree_coord_cur,
                                                      octree_offset_tables[layer_i],
                                                      octree_data_tables[layer_i]);
      
      if(extractBit(octree_data.x, index_int)) {
        if(extractBit(octree_data.y, index_int)) {
          light_data = obtainNodeFromSpatialHashFunction(octree_coord_next,
                                                         light_offset_tables[layer_i],
                                                         light_data_tables[layer_i]);
        }
        break;
      }
    }
  }

  uint offset = light_data.x;
  uint n_lights = light_data.y;

  for (uint i = offset; i < offset + n_lights; ++i) {
    light_acc += computeLight(lights[light_indices[i]], param);
  }

  // output result
  fragment_colour = vec3((vec3(0.1f) + (light_acc * 0.9)));
}
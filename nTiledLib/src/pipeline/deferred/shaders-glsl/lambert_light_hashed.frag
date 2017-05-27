#version 440

#define NUM_LIGHTS 3
#define OCTREE_DEPTH 0

// Fragment Output Buffers
// -----------------------------------------------------------------------------
//out vec4 fragment_colour;
out vec4 fragment_colour;


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


// -----------------------------------------------------------------------------
// MRT texture samplers
layout(binding = 0) uniform sampler2D diffuse_tex;
layout(binding = 1) uniform sampler2D normal_tex;
layout(binding = 2) uniform sampler2D depth_tex;

uniform mat4 perspective_matrix;
uniform mat4 inv_perspective_matrix;
uniform mat4 inv_camera_matrix;

uniform vec4 viewport;
uniform vec2 depth_range;


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
uniform vec3 octree_origin;



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


/*!
 * Compute the texture coordinates of this fragment.
 */
vec2 calcTextureCoordinates() {
    return gl_FragCoord.xy / viewport.zw;
}


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


vec3 getWorldCoordinates(in vec4 camera_coordinates) {
  vec4 raw_coords = inv_camera_matrix * camera_coordinates;
  vec4 coords = raw_coords / raw_coords.w;

  return coords.xyz;
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
  vec2 tex_coords = calcTextureCoordinates();
  vec3 diffuse_colour = texture(diffuse_tex, tex_coords).xyz;
    
  if (diffuse_colour.rgb == vec3(0.0f)) {
    fragment_colour = vec4(0.0f);
  } else {
    vec4 coords_camera_space = getCameraCoordinates(tex_coords);
    vec3 normal = normalize(texture(normal_tex, tex_coords)).xyz;
  
    vec3 light_acc = vec3(0.0f);

    GeometryParam param = GeometryParam(coords_camera_space,
                                        normal,
                                        diffuse_colour);

    // Retrieve the relevant lights.
    uvec2 light_data = uvec2(0, 0);

    // calculate the octree position
    vec3 fragment_octree_position =  
        getWorldCoordinates(coords_camera_space) - octree_origin;
    

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
    fragment_colour = vec4((vec3(0.1f) + (light_acc * 0.9)), 1.0);
  }
}
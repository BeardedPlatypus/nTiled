#version 440

#define NUM_LIGHTS 3
#define M_PI 3.1415926535897932384626433832795

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

// -----------------------------------------------------------------------------
// MRT texture samplers
layout(binding = 0) uniform sampler2D diffuse_tex;
layout(binding = 1) uniform sampler2D normal_tex;
layout(binding = 2) uniform sampler2D depth_tex;

// -----------------------------------------------------------------------------
// Position definition
uniform mat4 perspective_matrix;
uniform mat4 inv_perspective_matrix;

uniform vec4 viewport;
uniform vec2 depth_range;

// Function Definitions
// -----------------------------------------------------------------------------
vec3 computeCubeHelix(float lambda, 
                      float rotations,
                      float start_colour,
                      float hue) {
  float phi = 2 * M_PI * ( start_colour / 3 + rotations * lambda);
  float alpha = hue * lambda * (1 - lambda) * 0.5;

  float cos_phi = cos(phi);
  float sin_phi = sin(phi);

  return (vec3(lambda) + vec3(alpha * (-0.14861 * cos_phi +  1.78277 * sin_phi),
                              alpha * (-0.29227 * cos_phi + -0.90649 * sin_phi),
                              alpha * ( 1.97294 * cos_phi)));
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


// -----------------------------------------------------------------------------
// Main
// -----------------------------------------------------------------------------
void main() {
    vec2 tex_coords = calcTextureCoordinates();
    vec3 diffuse_colour = texture(diffuse_tex, tex_coords).xyz;
    
    if (diffuse_colour.rgb == vec3(0.0f)) {
        fragment_colour = vec4(vec3(0.0f), 1.0f);
    } else {
        // output result
        vec3 cube_helix = computeCubeHelix( 1.0, 
                                           -1.5, 
                                            0.5,
                                            1.2);
        fragment_colour = vec4(cube_helix, 1.0);
    }   
}

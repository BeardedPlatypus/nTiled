#version 440

#define NUM_LIGHTS 3

// Fragment Input Buffers
// -----------------------------------------------------------------------------
in vec4 fragment_position;
in vec3 fragment_normal;

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

// -----------------------------------------------------------------------------
layout (std140) uniform LightBlock {
    Light lights[NUM_LIGHTS];
};

//  Light Management
// -----------------------------------------------------------------------------
layout (std430, binding = 0) buffer LightGridBuffer {
    uvec2 tiles[];
};

layout (std430, binding = 1) buffer LightIndexBuffer {
    uint light_indices[];
};

uniform uvec2 tile_size;
uniform uint n_tiles_x;




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

    // compute the contribution of each light
    /*
    for (int i =0; i < NUM_LIGHTS; i++) {
        light_acc += computeLight(lights[i], param);
    }
    */
    // determine contributing lights
    vec2 screen_position = gl_FragCoord.xy - vec2(0.5f, 0.5f);
    uint tile_index = uint(floor(screen_position.x / tile_size.x) +
                           floor(screen_position.y / tile_size.y) * n_tiles_x);

    uint offset = tiles[tile_index].x;
    uint n_lights = tiles[tile_index].y;

    light_calculations = n_lights;
}

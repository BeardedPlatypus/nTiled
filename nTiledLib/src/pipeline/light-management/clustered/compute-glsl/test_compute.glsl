#version 440

layout(local_size_x = 1,
       local_size_y = 1) in;

layout (binding = 0, rgba8) coherent uniform image2D result;

void main() {
    imageStore(result, 
               ivec2(gl_GlobalInvocationID.xy), 
               vec4(1.0f,
                    0.5f, 
                    0.0f,
                    1.0f));
}
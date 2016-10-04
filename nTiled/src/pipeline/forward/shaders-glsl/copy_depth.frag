#version 440

uniform vec4 viewport;

layout(binding = 0) uniform sampler2D colour_tex;
layout(binding = 1) uniform sampler2D depth_tex;

out vec4 fragment_colour;

void main() {
    fragment_colour = vec4(texture(colour_tex, (gl_FragCoord.xy / viewport.zw)).xyz, 1.0f);
}

#version 440

out vec4 fragmentColor;

uniform float colour_intensity;

void main() {
    fragmentColor = vec4(colour_intensity, 
                         colour_intensity,
                         colour_intensity, 
                         0.3);
}

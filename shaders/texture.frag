#version 460 core

// Shades through texture sampling

out vec3 fragment_color;

in vec2 uv;

uniform sampler2D texture_0;

void main() {
   fragment_color = texture(texture_0, uv).rgb;
}

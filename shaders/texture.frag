#version 460 core

// Shades in a "solid" color, interpolated by the vertex

out vec3 fragment_color;

in vec2 uv;
in vec3 normal;

uniform sampler2D tex;

void main() {
  fragment_color = texture(tex, uv).rgb;
}

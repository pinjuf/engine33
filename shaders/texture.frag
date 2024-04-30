#version 460 core

// Shades in a "solid" color, interpolated by the vertex

out vec3 fragment_color;

in vec2 uv;

uniform sampler2D tex;

void main() {
  fragment_color = texture(tex, uv).rgb;
}

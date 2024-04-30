#version 460 core

// Shades in a "solid" color, interpolated by the vertex

out vec3 fragment_color;

in vec3 vertex_color;

void main() {
  fragment_color = vertex_color;
}

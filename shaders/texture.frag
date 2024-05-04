#version 460 core

// Shades in a "solid" color, interpolated by the vertex

out vec3 fragment_color;

in vec2 uv;
in vec3 normal;

uniform sampler2D e33_texture0;

vec3 sun_direction = vec3(1, 2, 1);

float ambient = 0.1;

void main() {
  vec3 alignment = (normal + normalize(sun_direction))/2;
  fragment_color = texture(e33_texture0, uv).rgb * float(ambient + (1-ambient) * abs(alignment));
}

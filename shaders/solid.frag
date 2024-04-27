#version 460 core

// Shades in a single solid color

out vec3 fragment_color;

uniform vec3 input_color;

void main(){
  fragment_color = input_color;
}

#version 460 core

// Literally just projects xy

layout(location = 0) in vec3 vertpos_model;

void main() {
    gl_Position.xyz = vertpos_model;
    gl_Position.w = 1.0;
}

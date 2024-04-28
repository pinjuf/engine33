#version 460 core

// Basic Model-View-Projection vertex shader

in vec3 vertpos_model;

uniform mat4 mvp;

void main() {
    gl_Position.xyzw = mvp * vec4(vertpos_model, 1.0f);
}

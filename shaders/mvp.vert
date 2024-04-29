#version 460 core

// Basic Model-View-Projection vertex shader

in vec3 vertpos_model;
in vec3 vertex_color;

out vec3 ivertex_color;

uniform mat4 mvp;

void main() {
    gl_Position = mvp * vec4(vertpos_model, 1.0f);
    ivertex_color = vertex_color;
}

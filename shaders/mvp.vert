#version 460 core

// Basic Model-View-Projection vertex shader

in vec3 vertpos_model;
in vec3 ivertex_color;
in vec2 vertuv;

out vec2 uv;
out vec3 vertex_color;

uniform mat4 mvp;

void main() {
    gl_Position = mvp * vec4(vertpos_model, 1.0f);

    uv = vertuv;
    vertex_color = ivertex_color;
}

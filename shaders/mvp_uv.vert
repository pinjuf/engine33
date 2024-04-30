#version 460 core

// Basic Model-View-Projection vertex shader

in vec3 vertpos_model;
in vec2 vertuv;

out vec2 uv;

uniform mat4 mvp;

void main() {
    gl_Position = mvp * vec4(vertpos_model, 1.0f);

    uv = vertuv;
}

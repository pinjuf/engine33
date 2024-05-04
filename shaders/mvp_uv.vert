#version 460 core

// Basic Model-View-Projection vertex shader

in vec3 e33_vertexposition;
in vec2 e33_vertexuv;
in vec3 e33_vertexnormal;

out vec2 uv;
out vec3 normal;

uniform mat4 e33_mvp;

void main() {
    gl_Position = e33_mvp * vec4(e33_vertexposition, 1.0f);

    uv = e33_vertexuv;
    normal = e33_vertexnormal;
}

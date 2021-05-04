#version 330 core

vec2 positions[6] = vec2[](
    vec2(-1.0f, -1.0f),
    vec2(1.0f, -1.0f),
    vec2(1.0f, 1.0f), //left bottom
    vec2(-1.0f, -1.0f),
    vec2(1.0f, 1.0f),
    vec2(-1.0f, 1.0f) //right top
);

out vec2 TexCoord;

void main() {
    vec2 position = positions[gl_VertexID];
    gl_Position = vec4(position, 0.0, 1.0);

    TexCoord = (position + 1.0) / 2;
}

#version 330 core

layout(location = 0) in vec2 pos;

uniform mat4 projection;
uniform mat4 modelView;

void main() {
    gl_Position = projection * modelView * vec4(pos, 0.0, 1.0);
}

#version 330 core

in layout(location = 0) vec2 pos;

uniform mat4 projection;
uniform mat4 modelView;

void main() {
    gl_Position = projection * modelView * vec4(pos, 0.0, 1.0);
}

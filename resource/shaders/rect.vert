#version 330 core

in layout(location = 0) vec2 pos;

void main() {
    gl_Position = vec4(pos, 0.0, 1.0);
}

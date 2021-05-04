#version 330 core

uniform sampler2D screen;
uniform vec4 color;

in vec2 TexCoord;

out vec4 FragColor;

void main() {
    FragColor = texture(screen, TexCoord) * color;
}

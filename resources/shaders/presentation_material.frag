#version 330 core

in vec2 texture_coordinates;
out vec4 color;

uniform sampler2D backbuffer;

void main()
{
    color = texture(backbuffer, texture_coordinates);
}
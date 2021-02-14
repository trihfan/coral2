#version 330 core

out vec4 fColor;

uniform sampler2D backbuffer;

void main()
{
    fColor = texelFetch(backbuffer, ivec2(gl_FragCoord.x, gl_FragCoord.y), 0);
}
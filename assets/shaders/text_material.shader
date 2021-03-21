canvas

[vertex]

// Varyings
out vec2 textCoords;

void main()
{
    textCoords = VERTEX.textCoords;
    gl_Position = PROJECTION_MATRIX * vec4(VERTEX.position, 1);
}

[fragment]

// Uniforms
uniform sampler2D glyph;
uniform vec3 color;

// Varyings
in vec2 textCoords;

void main()
{    
    fragColor0 = vec4(color, texture(glyph, textCoords).r);
}

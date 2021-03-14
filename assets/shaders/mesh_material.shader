basic_lighting

[vertex]

out vec3 position;
out vec3 normal;

void main()
{
#ifdef SKINING
    position = vec3(MODEL_MATRIX * vec4(VERTEX.position, 1));
#else
    position = vec3(MODEL_MATRIX * vec4(VERTEX.position, 1));
#endif

    normal = mat3(transpose(inverse(MODEL_MATRIX))) * VERTEX.normal;
    gl_Position = PROJECTION_MATRIX * VIEW_MATRIX * vec4(position, 1);
}

[fragment]

// Uniforms
struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

in vec3 position;
in vec3 normal;

void main()
{
    vec3 fragmentNormal = normalize(normal);
    vec3 result = computeLighting(material.ambient, material.diffuse, material.specular, material.shininess, fragmentNormal, position);
    fragColor0 = vec4(result, 1);
}

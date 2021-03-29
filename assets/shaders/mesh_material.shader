basic_lighting

[vertex]

// Uniforms
#ifdef SKINING
uniform mat4 finalBoneMatrices[MAX_BONES];
#endif 

// Varyings
out vec3 position;
out vec3 normal;
out vec2 textCoords;

#ifdef SKINING
void computeSkining()
{
    vec4 totalPosition = vec4(0, 0, 0, 1);
    int count = 0;

    for (int i = 0; i < 4; i++)
    {
        int boneId = floatBitsToInt(VERTEX.bone[i]);
        if (boneId < MAX_BONES && boneId >= 0) 
        {
            totalPosition += finalBoneMatrices[boneId] * vec4(VERTEX.position, 1) * VERTEX.weight[i];
            count++;
        }

    }

    if (count > 0)
    {
        position = vec3(MODEL_MATRIX * totalPosition);
    }
    else
    {
    position = vec3(MODEL_MATRIX * vec4(VERTEX.position, 1));
    }

    normal = mat3(transpose(inverse(MODEL_MATRIX))) * VERTEX.normal;
}
#endif

void main()
{
#ifdef SKINING
    computeSkining();
#else
    position = vec3(MODEL_MATRIX * vec4(VERTEX.position, 1));
    normal = mat3(transpose(inverse(MODEL_MATRIX))) * VERTEX.normal;
#endif
 
    textCoords = VERTEX.textCoords;
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

#ifdef TEXTURING
uniform sampler2D texture_ambient0;
uniform sampler2D texture_diffuse0;
uniform sampler2D texture_specular0;
#endif

// Varyings
in vec3 position;
in vec3 normal;
in vec2 textCoords;

void main()
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    #ifdef TEXTURING
        ambient = texture(texture_ambient0, textCoords).rgb;
        diffuse = texture(texture_diffuse0, textCoords).rgb;
        specular = texture(texture_specular0, textCoords).rgb;
    #else
        ambient = material.ambient;
        diffuse = material.diffuse;
        specular = material.specular;
    #endif

    vec3 fragmentNormal = normalize(normal);
    vec3 result = computeLighting(material.ambient, diffuse, material.specular, material.shininess, fragmentNormal, position);
    fragColor0 = vec4(result, 1);
}

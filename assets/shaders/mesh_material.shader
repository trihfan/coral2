basic_lighting

[vertex]

// Uniforms
#ifdef SKINING
UNIFORM_BLOCK(Skeleton, 64)
{
    uniform mat4 finalBoneMatrices[MAX_BONES];
} skeleton;
#endif 

// Varyings
OUT vec3 position;
OUT vec3 normal;
OUT vec2 textCoords;

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
            totalPosition += skeleton.finalBoneMatrices[boneId] * vec4(VERTEX.position, 1) * VERTEX.weight[i];
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
    POSITION = PROJECTION_MATRIX * VIEW_MATRIX * vec4(position, 1);
}

[fragment]

// Uniforms
UNIFORM_BLOCK(Material, 40)
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
} material;

#ifdef TEXTURING
UNIFORM_BLOCK(Textures, 12)
{
    uniform sampler2D texture_ambient0;
    uniform sampler2D texture_diffuse0;
    uniform sampler2D texture_specular0;
} textures;
#endif

// Varyings
IN vec3 position;
IN vec3 normal;
IN vec2 textCoords;

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
    COLOR_0 = vec4(result, 1);
}

basic_lighting

[vertex]

// Uniforms
const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

// Varyings
out vec3 position;
out vec3 normal;

#ifdef SKINING
vec4 computeSkining()
{
    vec4 totalPosition = vec4(0);
    for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
    {
        if (boneIds[i] == -1) 
        {
            continue;
        }

        if (boneIds[i] >= MAX_JOINTS) 
        {
            totalPosition = vec4(VERTEX.position, 1);
            break;
        }

        vec4 localPosition = finalBoneMatrices[boneIds[i]] * vec4(VERTEX.position, 1);
        totalPosition += localPosition * weights[i];
        vec3 localNormal = mat3(finalBoneMatrices[boneIds[i]]) * VERTEX.normal;
   }
   return totalPosition;
}
#endif

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

// Varyings
in vec3 position;
in vec3 normal;

void main()
{
    vec3 fragmentNormal = normalize(normal);
    vec3 result = computeLighting(material.ambient, material.diffuse, material.specular, material.shininess, fragmentNormal, position);
    fragColor0 = vec4(result, 1);
}

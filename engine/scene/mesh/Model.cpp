#include "Model.h"
#include "Bone.h"
#include "ObjectFactory.h"
#include "materials/BasicMaterial.h"
#include "materials/MeshMaterial.h"
#include "materials/TexturedMaterial.h"
#include "utils/AssimpHelpers.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#define MAX_BONE_WEIGHTS 100

using namespace coral;

std::vector<MeshTextureType> assimpToMeshTextureType {
    MeshTextureType::unknown, //aiTextureType_NONE = 0,
    MeshTextureType::diffuse, //aiTextureType_DIFFUSE = 1,
    MeshTextureType::specular, //aiTextureType_SPECULAR = 2,
    MeshTextureType::height, //aiTextureType_AMBIENT = 3,
    MeshTextureType::unknown, //aiTextureType_EMISSIVE = 4,
    MeshTextureType::normal, //aiTextureType_HEIGHT = 5,
    MeshTextureType::unknown, //aiTextureType_NORMALS = 6,
    MeshTextureType::unknown, //aiTextureType_SHININESS = 7,
    MeshTextureType::unknown, //aiTextureType_OPACITY = 8,
    MeshTextureType::unknown, //aiTextureType_DISPLACEMENT = 9,
    MeshTextureType::unknown, //aiTextureType_LIGHTMAP = 10,
    MeshTextureType::unknown, //aiTextureType_REFLECTION = 11,
    MeshTextureType::unknown, //aiTextureType_BASE_COLOR = 12,
    MeshTextureType::unknown, //aiTextureType_NORMAL_CAMERA = 13,
    MeshTextureType::unknown, //aiTextureType_EMISSION_COLOR = 14,
    MeshTextureType::unknown, //aiTextureType_METALNESS = 15,
    MeshTextureType::unknown, //aiTextureType_DIFFUSE_ROUGHNESS = 16,
    MeshTextureType::unknown, //aiTextureType_AMBIENT_OCCLUSION = 17,
};

Model::Model(const std::string& path)
    : path(path)
{
    addRenderQueueTag(defaultRenderPassName);
}

void Model::init()
{
    Node::init();

    // Read the file
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);

    // Check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        Logs(error) << "Assimp error " << importer.GetErrorString();
        return;
    }

    // Retrieve the directory path of the filepath
    directory = path.substr(0, path.find_last_of('/'));

    // Load nodes
    loadNode(scene->mRootNode, scene);

    // Load animations
    loadAnimations(scene);

    // Clear data holder
    materialByName.clear();
}

void Model::loadNode(aiNode* node, const aiScene* scene)
{
    // Meshes
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];
        auto mesh = loadMesh(aiMesh, scene);
        meshByName[mesh->getName()].push_back(mesh);
        addChild(mesh);
    }

    // Children nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        loadNode(node->mChildren[i], scene);
    }
}

void Model::loadAnimations(const aiScene* scene)
{
    // Read missing bones from animation data
    for (unsigned int i = 0; i < scene->mNumAnimations; i++)
    {
        auto animation = scene->mAnimations[i];
        for (unsigned int j = 0; j < animation->mNumChannels; j++)
        {
            auto channel = animation->mChannels[j];
            std::string boneName = channel->mNodeName.data;
            if (boneInfoMap.find(boneName) == boneInfoMap.end())
            {
                boneInfoMap[boneName].id = boneCounter++;
            }
        }
    }

    // Read animation nodes
    std::function<void(Handle<AnimationNode>, const aiNode*)> readAnimationNode = [&](Handle<AnimationNode> node, const aiNode* src) {
        node->name = src->mName.data;
        node->transformation = AssimpHelpers::convertMatrixToGLMFormat(src->mTransformation);

        for (unsigned int i = 0; i < src->mNumChildren; i++)
        {
            auto child = ObjectFactory::create<AnimationNode>();
            readAnimationNode(child, src->mChildren[i]);
            node->children.push_back(child);
        }
    };
    auto animationRootNode = ObjectFactory::create<AnimationNode>();
    readAnimationNode(animationRootNode, scene->mRootNode);

    // Create animations
    for (unsigned int i = 0; i < scene->mNumAnimations; i++)
    {
        auto animation = scene->mAnimations[i];

        AnimationParams params;
        params.rootNode = animationRootNode;
        params.boneInfoMap = boneInfoMap;
        params.duration = static_cast<float>(animation->mDuration);
        params.ticksPerSecond = static_cast<float>(animation->mTicksPerSecond);

        // Fill bones list
        params.bones.resize(animation->mNumChannels);
        for (unsigned int j = 0; j < animation->mNumChannels; j++)
        {
            auto channel = animation->mChannels[j];
            params.bones[j] = ObjectFactory::create<Bone>(channel->mNodeName.data, boneInfoMap[channel->mNodeName.data].id, channel);
        }

        animations.push_back(ObjectFactory::create<Animation>(params));
    }
}

Handle<Mesh> Model::loadMesh(aiMesh* mesh, const aiScene* scene)
{
    // Load vertices
    MeshVertexBuffer vertices;
    vertices.reserve(mesh->mNumVertices);
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        // Position
        vertices.insert(MeshVertexBuffer::position, glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));

        // Normal
        if (mesh->HasNormals())
        {
            vertices.insert(MeshVertexBuffer::normal, glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));
        }

        // Texture coordinates
        if (mesh->mTextureCoords[0])
        {
            vertices.insert(MeshVertexBuffer::textCoords, glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y));
        }

        // Bones
        if (mesh->mNumBones > 0)
        {
            vertices.insert(MeshVertexBuffer::bone, glm::ivec4(-1, -1, -1, -1));
            vertices.insert(MeshVertexBuffer::weight, glm::vec4());
        }
    }

    // Load bones binded to vertices
    for (unsigned int boneIndex = 0; boneIndex < mesh->mNumBones; boneIndex++)
    {
        int boneID = -1;
        std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();

        // Put to map
        if (boneInfoMap.find(boneName) == boneInfoMap.end())
        {
            BoneInfo newBoneInfo;
            newBoneInfo.id = boneCounter++;
            newBoneInfo.offset = AssimpHelpers::convertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix);
            boneInfoMap[boneName] = newBoneInfo;
        }

        boneID = boneInfoMap[boneName].id;

        // Get weights
        auto weights = mesh->mBones[boneIndex]->mWeights;
        unsigned int numWeights = mesh->mBones[boneIndex]->mNumWeights;

        // Add weight/bone to the vertex
        for (unsigned int weightIndex = 0; weightIndex < numWeights; weightIndex++)
        {
            unsigned int vertexId = weights[weightIndex].mVertexId;
            float weight = weights[weightIndex].mWeight;

            // Get current bone data
            glm::ivec4 bones;
            glm::vec4 weights;
            vertices.get(MeshVertexBuffer::bone, vertexId, bones);
            vertices.get(MeshVertexBuffer::weight, vertexId, weights);

            // Max 4 bones can impact a vertex
            for (int i = 0; i < 4; ++i)
            {
                if (bones[i] < 0)
                {
                    weights[i] = weight;
                    bones[i] = boneID;
                    break;
                }
            }

            // Update the vertex
            vertices.set(MeshVertexBuffer::bone, vertexId, bones);
            vertices.set(MeshVertexBuffer::weight, vertexId, weights);
        }
    }

    // Load indices
    std::vector<unsigned int> indices;
    indices.reserve(mesh->mNumFaces * 3);
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    // Load the material
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    Handle<Material> meshMaterial = loadMaterial(material, scene, vertices);

    // Create the mesh object
    std::string name = mesh->mName.C_Str();
    Handle<Mesh> meshObject = ObjectFactory::createWithName<Mesh>(name, vertices, indices);
    meshObject->setMaterial(meshMaterial);

    return meshObject;
}

Handle<Material> Model::loadMaterial(aiMaterial* mat, const aiScene* scene, const MeshVertexBuffer& vertexBuffer)
{
    // Check if the material is already loaded
    std::string name = mat->GetName().C_Str();
    auto it = materialByName.find(name);
    if (it != materialByName.end())
    {
        return it->second;
    }

    // Create material
    Handle<MeshMaterial> material = ObjectFactory::create<MeshMaterial>(getRenderQueueTags());
    material->setRenderType(MeshMaterialRenderType::basic_lighting);

    // Attributes
    std::vector<ShaderAttribute> attributes;
    attributes.push_back(ShaderAttribute { "position", vertexBuffer.getLocation(MeshVertexBuffer::position), "vec3" });
    if (vertexBuffer.hasAttribute(MeshVertexBuffer::normal))
    {
        attributes.push_back(ShaderAttribute { "normal", vertexBuffer.getLocation(MeshVertexBuffer::normal), "vec3" });
    }
    if (vertexBuffer.hasAttribute(MeshVertexBuffer::textCoords))
    {
        attributes.push_back(ShaderAttribute { "textCoords", vertexBuffer.getLocation(MeshVertexBuffer::textCoords), "vec2" });
    }
    if (vertexBuffer.hasAttribute(MeshVertexBuffer::bone))
    {
        attributes.push_back(ShaderAttribute { "bone", vertexBuffer.getLocation(MeshVertexBuffer::bone), "ivec4" });
        material->enableSkining();
    }
    if (vertexBuffer.hasAttribute(MeshVertexBuffer::weight))
    {
        attributes.push_back(ShaderAttribute { "weight", vertexBuffer.getLocation(MeshVertexBuffer::weight), "vec4" });
    }
    material->setAttributes(attributes);

    // Textures
    if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0)
    {
        material->enableTexturing();
        for (size_t i = 0; i < assimpToMeshTextureType.size(); i++)
        {
            if (assimpToMeshTextureType[i] != MeshTextureType::unknown)
            {
                for (unsigned int j = 0; j < mat->GetTextureCount(static_cast<aiTextureType>(i)); j++)
                {
                    aiString str;
                    mat->GetTexture(static_cast<aiTextureType>(i), j, &str);
                    material->addTexture(assimpToMeshTextureType[i], loadTexture(scene, str.C_Str()));
                }
            }
        }
    }

    // Basic colors
    // Ambient color
    aiColor3D color(0, 0, 0);
    if (mat->Get(AI_MATKEY_COLOR_AMBIENT, color) == aiReturn_SUCCESS)
    {
        material->setAmbientColor(glm::vec3(color.r, color.g, color.b));
    }

    // Diffuse color
    if (mat->Get(AI_MATKEY_COLOR_DIFFUSE, color) == aiReturn_SUCCESS)
    {
        material->setDiffuseColor(glm::vec3(color.r, color.g, color.b));
    }

    // Specular color
    if (mat->Get(AI_MATKEY_COLOR_SPECULAR, color) == aiReturn_SUCCESS)
    {
        material->setSpecularColor(glm::vec3(color.r, color.g, color.b));
    }

    // Shininess
    float shininess;
    if (mat->Get(AI_MATKEY_SHININESS, shininess) == aiReturn_SUCCESS)
    {
        material->setShininess(shininess);
    }

    return material;
}

Handle<Resource> Model::loadTexture(const aiScene* scene, const std::string& file)
{
    auto texture = scene->GetEmbeddedTexture(file.c_str());

    // File texture
    if (!texture)
    {
        std::string resourcePath = directory + "/" + file;
        return ObjectFactory::createWithName<Resource>(file, resourcePath);
    }

    // Embedded texture uncompressed
    if (texture->mHeight != 0)
    {
        ResourceParams params;
        params.type = backend::BackendResourceType::texture2d;
        params.width = static_cast<int>(texture->mWidth);
        params.height = static_cast<int>(texture->mHeight);

        params.format = backend::BackendResourceFormat::argb8888;
        params.data.resize(static_cast<size_t>(params.width * params.height) * sizeof(aiTexel));
        std::memcpy(params.data.data(), texture->pcData, params.data.size());
        return ObjectFactory::createWithName<Resource>(file, params);
    }

    // Embedded texture compressed
    return ObjectFactory::createWithName<Resource>(file, reinterpret_cast<const unsigned char*>(texture->pcData), texture->mWidth);
}

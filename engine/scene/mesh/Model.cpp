#include "Model.h"
#include "Bone.h"
#include "base/ObjectFactory.h"
#include "materials/MeshMaterial.h"
#include "resources/ShaderComposer.h"
#include "scene/animation/Animation.h"
#include "utils/AssimpHelpers.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#define MAX_BONE_WEIGHTS 100

static const std::vector<std::string> attributeNames {
    "position",
    "normal",
    "textCoords",
    "bone",
    "weight"
};

static const std::vector<std::string> attributeTypes {
    "vec3",
    "vec3",
    "vec2",
    "vec4",
    "vec4"
};

using namespace coral;

static std::vector<MeshTextureType> assimpToMeshTextureType {
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
    MeshTextureType::unknown //aiTextureType_AMBIENT_OCCLUSION = 17,
};

Model::Model(const std::string& path)
    : skeleton(ObjectFactory::create<Node>())
{
    addRenderQueueTag(defaultRenderPassName);

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
    loadNode(nullptr, scene->mRootNode, scene);

    // Load animations
    loadAnimations(scene);

    // Build the skeleton
    buildSkeleton(scene);

    // Pass bones to materials
    for (auto bone : bones)
    {
        for (auto material : materialByName)
        {
            material.second->setBone(bone.second->getId(), bone.second);
        }
    }

    // Clear data holder
    bones.clear();
    materialByName.clear();
}

void Model::update(const NodeUpdateParameters& parameters)
{
    Node::update(parameters);
    skeleton->update(parameters);
}

void Model::loadNode(ptr<Node> parent, aiNode* node, const aiScene* scene)
{
    // Create node
    ptr<Node> sceneNode = ObjectFactory::createWithName<Node>(node->mName.C_Str());
    if (parent)
    {
        parent->addChild(sceneNode);
    }
    else
    {
        addChild(sceneNode);
    }

    // Extract tranform
    sceneNode->transform().setMatrix(AssimpHelpers::convertMatrixToGLMFormat(node->mTransformation));

    // Meshes
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        ptr<Mesh> mesh = loadMesh(scene->mMeshes[node->mMeshes[i]], scene);
        sceneNode->addChild(mesh);
    }

    // Children nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        loadNode(sceneNode, node->mChildren[i], scene);
    }
}

void Model::loadAnimations(const aiScene* scene)
{
    // Create animations
    for (unsigned int i = 0; i < scene->mNumAnimations; i++)
    {
        auto aiAnimation = scene->mAnimations[i];
        Logs(info) << "Animation found: " << aiAnimation->mName.C_Str();

        // Create animation
        ptr<Animation> animation = ObjectFactory::createWithName<ModelAnimation>(aiAnimation->mName.C_Str(), aiAnimation->mDuration / 1000., skeleton);

        // Fill bones list
        for (unsigned int j = 0; j < aiAnimation->mNumChannels; j++)
        {
            auto aiChannel = aiAnimation->mChannels[j];

            // Get the bone
            std::string boneName = aiChannel->mNodeName.data;
            if (bones.find(boneName) == bones.end())
            {
                bones[boneName] = ObjectFactory::createWithName<Bone>(boneName, boneCounter++);
            }
            ptr<Bone> bone = bones[boneName];

            // Each bone has 3 properties
            auto translationChannel = ObjectFactory::createWithName<Channel<glm::vec3>>(boneName + "-translation", bone->transform().translation);
            auto rotationChannel = ObjectFactory::createWithName<Channel<glm::quat, SphericalLinearInterpolator>>(boneName + "-rotation", bone->transform().rotation);
            auto scaleChannel = ObjectFactory::createWithName<Channel<glm::vec3>>(boneName + "-scale", bone->transform().scale);

            // Read keyframes
            for (size_t i = 0; i < aiChannel->mNumPositionKeys; i++)
            {
                aiVector3D aiPosition = aiChannel->mPositionKeys[i].mValue;
                double timeStamp = aiChannel->mPositionKeys[i].mTime / 1000.;
                translationChannel->addKeyframe({ timeStamp, AssimpHelpers::getGLMVec(aiPosition) });
            }

            for (size_t i = 0; i < aiChannel->mNumRotationKeys; i++)
            {
                aiQuaternion aiOrientation = aiChannel->mRotationKeys[i].mValue;
                double timeStamp = aiChannel->mRotationKeys[i].mTime / 1000.;
                rotationChannel->addKeyframe({ timeStamp, AssimpHelpers::getGLMQuat(aiOrientation) });
            }

            for (size_t i = 0; i < aiChannel->mNumScalingKeys; i++)
            {
                aiVector3D scale = aiChannel->mScalingKeys[i].mValue;
                double timeStamp = aiChannel->mScalingKeys[i].mTime / 1000.;
                scaleChannel->addKeyframe({ timeStamp, AssimpHelpers::getGLMVec(scale) });
            }

            animation->addChannel(translationChannel);
            animation->addChannel(rotationChannel);
            animation->addChannel(scaleChannel);
        }

        animations.push_back(animation);
    }
}

ptr<Mesh> Model::loadMesh(aiMesh* mesh, const aiScene* scene)
{
    // Load vertices
    MeshVertexBuffer vertices;
    vertices.reserve(mesh->mNumVertices);
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        // Position
        vertices.addPosition(glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));

        // Normal
        if (mesh->HasNormals())
        {
            vertices.addNormal(glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));
        }

        // Texture coordinates
        if (mesh->mTextureCoords[0])
        {
            vertices.addTextCoord(glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y));
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

    // Load bones
    for (unsigned int boneIndex = 0; boneIndex < mesh->mNumBones; boneIndex++)
    {
        // Get or create the bone
        std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
        if (bones.find(boneName) == bones.end())
        {
            bones[boneName] = ObjectFactory::createWithName<Bone>(boneName, boneCounter++, AssimpHelpers::convertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix));
        }
        ptr<Bone> bone = bones[boneName];

        // Get weights
        auto weights = mesh->mBones[boneIndex]->mWeights;
        unsigned int numWeights = mesh->mBones[boneIndex]->mNumWeights;

        // Add weight/bone to the vertex
        for (unsigned int weightIndex = 0; weightIndex < numWeights; weightIndex++)
        {
            unsigned int vertexId = weights[weightIndex].mVertexId;
            float weight = weights[weightIndex].mWeight;
            vertices.addBoneIncidence(vertexId, bone->getId(), weight);
        }
    }

    // Create the mesh object
    std::string name = mesh->mName.C_Str();
    ptr<Mesh> meshObject = ObjectFactory::createWithName<Mesh>(name, vertices, indices);

    // Load the material
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    ptr<Material> meshMaterial = loadMaterial(material, scene, vertices);
    meshObject->setMaterial(meshMaterial);

    return meshObject;
}

ptr<Material> Model::loadMaterial(aiMaterial* mat, const aiScene* scene, const MeshVertexBuffer& vertexBuffer)
{
    // Check if the material is already loaded
    std::string name = mat->GetName().C_Str();
    auto it = materialByName.find(name);
    if (it != materialByName.end())
    {
        return it->second;
    }

    // Create material
    ptr<MeshMaterial> material = ObjectFactory::create<MeshMaterial>(getRenderQueueTags());
    materialByName[name] = material;
    material->setRenderType(MeshMaterialRenderType::basic_lighting);

    // Attributes
    std::vector<ShaderAttribute> attributes;
    for (size_t i = 0; i < MeshVertexBuffer::count; i++)
    {
        if (vertexBuffer.hasAttribute(static_cast<MeshVertexBuffer::AttributeType>(i)))
        {
            attributes.push_back(ShaderAttribute { attributeNames[i], vertexBuffer.getLocation(static_cast<MeshVertexBuffer::AttributeType>(i)), attributeTypes[i] });
        }
    }
    material->setAttributes(attributes);

    // Parameters
    if (vertexBuffer.hasAttribute(MeshVertexBuffer::boneId))
    {
        material->enableSkining();
    }

    // Textures
    /*if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0)
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
    }*/

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

ptr<Resource> Model::loadTexture(const aiScene* scene, const std::string& file)
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

std::vector<std::string> Model::getAnimationNames() const
{
    std::vector<std::string> names(animations.size());
    for (size_t i = 0; i < names.size(); i++)
    {
        names[i] = animations[i]->getName();
    }
    return names;
}

ptr<Animation> Model::getAnimation(const std::string& animationName) const
{
    auto it = std::find_if(animations.begin(), animations.end(), [&animationName](const ptr<Animation>& animation) {
        return animation->getName() == animationName;
    });

    if (it != animations.end())
    {
        return *it;
    }
    return nullptr;
}

void Model::buildSkeleton(const aiScene* scene)
{
    std::function<void(ptr<Node>, const aiNode*)> readAnimationNode = [&](ptr<Node> parent, const aiNode* aiParent) {
        for (unsigned int i = 0; i < aiParent->mNumChildren; i++)
        {
            auto aiChild = aiParent->mChildren[i];
            auto it = bones.find(aiChild->mName.C_Str());

            // If there is a bone
            if (it != bones.end())
            {
                auto bone = it->second;
                parent->addChild(bone);
                readAnimationNode(bone, aiChild);
            }
            // Otherwise
            else
            {
                auto child = ObjectFactory::createWithName<Node>(aiChild->mName.C_Str());
                parent->addChild(child);
                child->transform().setMatrix(AssimpHelpers::convertMatrixToGLMFormat(aiChild->mTransformation));
                readAnimationNode(child, aiChild);
            }
        }
    };

    skeleton->setName(scene->mRootNode->mName.C_Str());
    skeleton->transform().setMatrix(AssimpHelpers::convertMatrixToGLMFormat(scene->mRootNode->mTransformation));
    readAnimationNode(skeleton, scene->mRootNode);
}

ModelAnimation::ModelAnimation(double duration, ptr<Node> skeleton)
    : Animation(duration)
    , skeleton(skeleton)
{
}

void ModelAnimation::start()
{
    for (auto bone : skeleton->getChildren<Bone>())
    {
        bone->setAnimated(true);
    }
}

void ModelAnimation::stop()
{
    for (auto bone : skeleton->getChildren<Bone>())
    {
        bone->setAnimated(false);
    }
}
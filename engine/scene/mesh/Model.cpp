#include "Model.h"
#include "ObjectFactory.h"
#include "materials/BasicMaterial.h"
#include "materials/MeshMaterial.h"
#include "materials/TexturedMaterial.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

using namespace coral;

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
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    // Check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        Logs(error) << "Assimp error " << importer.GetErrorString();
        return;
    }

    // Retrieve the directory path of the filepath
    directory = path.substr(0, path.find_last_of('/'));

    // Load nodes
    loadNode(scene->mRootNode, scene);

    // Clear data holder
    materialByName.clear();
}

void Model::loadNode(aiNode* node, const aiScene* scene)
{
    // Meshes
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        auto meshm = loadMesh(mesh, scene);
        addChild(meshm);
    }

    // Children nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        loadNode(node->mChildren[i], scene);
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
    Handle<Material> meshMaterial = loadMaterial(material, vertices);

    // Create the mesh object
    std::string name = mesh->mName.C_Str();
    Handle<Mesh> meshObject = ObjectFactory::createWithName<Mesh>(name, vertices, indices);
    meshObject->setMaterial(meshMaterial);
    return meshObject;
}

Handle<Material> Model::loadMaterial(aiMaterial* mat, const MeshVertexBuffer& vertexBuffer)
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
    if (vertexBuffer.has(MeshVertexBuffer::normal))
    {
        attributes.push_back(ShaderAttribute { "normal", vertexBuffer.getLocation(MeshVertexBuffer::normal), "vec3" });
    }
    if (vertexBuffer.has(MeshVertexBuffer::textCoords))
    {
        attributes.push_back(ShaderAttribute { "textCoords", vertexBuffer.getLocation(MeshVertexBuffer::textCoords), "vec2" });
    }
    if (vertexBuffer.has(MeshVertexBuffer::bone))
    {
        attributes.push_back(ShaderAttribute { "bone", vertexBuffer.getLocation(MeshVertexBuffer::bone), "ivec4" });
        material->enableSkining();
    }
    if (vertexBuffer.has(MeshVertexBuffer::weight))
    {
        attributes.push_back(ShaderAttribute { "weight", vertexBuffer.getLocation(MeshVertexBuffer::weight), "vec4" });
    }
    material->setAttributes(attributes);

    // Diffuse map
    for (unsigned int i = 0; i < mat->GetTextureCount(aiTextureType_DIFFUSE); i++)
    {
        std::string resourcePath = directory + "/" + name;
        material->addTexture(MeshTextureType::diffuse, ObjectFactory::createWithName<Resource>(name, resourcePath));
    }

    // Specular map
    for (unsigned int i = 0; i < mat->GetTextureCount(aiTextureType_SPECULAR); i++)
    {
        std::string resourcePath = directory + "/" + name;
        material->addTexture(MeshTextureType::specular, ObjectFactory::createWithName<Resource>(name, resourcePath));
    }

    // Normal map
    for (unsigned int i = 0; i < mat->GetTextureCount(aiTextureType_HEIGHT); i++)
    {
        std::string resourcePath = directory + "/" + name;
        material->addTexture(MeshTextureType::normal, ObjectFactory::createWithName<Resource>(name, resourcePath));
    }

    // Height map
    for (unsigned int i = 0; i < mat->GetTextureCount(aiTextureType_AMBIENT); i++)
    {
        std::string resourcePath = directory + "/" + name;
        material->addTexture(MeshTextureType::height, ObjectFactory::createWithName<Resource>(name, resourcePath));
    }

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

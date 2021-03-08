#include "Model.h"
#include "ObjectFactory.h"
#include "materials/BasicMaterial.h"
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
        vertices.positions.push_back(glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));

        // Normal
        if (mesh->HasNormals())
        {
            vertices.normals.push_back(glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));
        }

        // Texture coordinates
        if (mesh->mTextureCoords[0])
        {
            vertices.textCoords.push_back(glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y));
        }

        // Tangent
        if (mesh->mTangents)
        {
            vertices.tangents.push_back(glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z));
        }

        // Bitangent
        if (mesh->mBitangents)
        {
            vertices.bitangents.push_back(glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z));
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
    Handle<Material> meshMaterial = loadMaterial(material);

    // Create the mesh object
    std::string name = mesh->mName.C_Str();
    Handle<Mesh> meshObject = ObjectFactory::createWithName<Mesh>(name, vertices, indices);
    meshObject->setMaterial(meshMaterial);
    return meshObject;
}

Handle<Material> Model::loadMaterial(aiMaterial* mat)
{
    // Check if the material is already loaded
    std::string name = mat->GetName().C_Str();
    auto it = materialByName.find(name);
    if (it != materialByName.end())
    {
        return it->second;
    }

    // -----------------------
    // -- Textured Material --
    if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0)
    {
        Handle<TexturedMaterial> meshMaterial = ObjectFactory::createWithName<TexturedMaterial>(name, getRenderQueueTags());

        // Diffuse map
        for (unsigned int i = 0; i < mat->GetTextureCount(aiTextureType_DIFFUSE); i++)
        {
            std::string resourcePath = directory + "/" + name;
            meshMaterial->diffuseTextures.push_back(ObjectFactory::createWithName<Resource>(name, resourcePath));
        }

        // Specular map
        for (unsigned int i = 0; i < mat->GetTextureCount(aiTextureType_SPECULAR); i++)
        {
            std::string resourcePath = directory + "/" + name;
            meshMaterial->specularTextures.push_back(ObjectFactory::createWithName<Resource>(name, resourcePath));
        }

        // Normal map
        for (unsigned int i = 0; i < mat->GetTextureCount(aiTextureType_HEIGHT); i++)
        {
            std::string resourcePath = directory + "/" + name;
            meshMaterial->normalTextures.push_back(ObjectFactory::createWithName<Resource>(name, resourcePath));
        }

        // Height map
        for (unsigned int i = 0; i < mat->GetTextureCount(aiTextureType_AMBIENT); i++)
        {
            std::string resourcePath = directory + "/" + name;
            meshMaterial->heightTextures.push_back(ObjectFactory::createWithName<Resource>(name, resourcePath));
        }

        return meshMaterial;
    }

    // --------------------
    // -- Basic Material --
    Handle<BasicMaterial> meshMaterial = ObjectFactory::createWithName<BasicMaterial>(name, getRenderQueueTags());

    // Ambient color
    aiColor3D ambient(0, 0, 0);
    mat->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
    meshMaterial->ambient = glm::vec3(ambient.r, ambient.g, ambient.b);

    // Diffuse color
    aiColor3D diffuse(0, 0, 0);
    mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
    meshMaterial->diffuse = glm::vec3(diffuse.r, diffuse.g, diffuse.b);

    // Specular color
    aiColor3D specular(0, 0, 0);
    mat->Get(AI_MATKEY_COLOR_DIFFUSE, specular);
    meshMaterial->specular = glm::vec3(specular.r, specular.g, specular.b);

    // Shininess
    float shininess;
    mat->Get(AI_MATKEY_SHININESS, shininess);
    meshMaterial->shininess = shininess;

    return meshMaterial;
}

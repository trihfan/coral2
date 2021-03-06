#include "Model.h"
#include "ObjectFactory.h"
#include "materials/BasicMaterial.h"
#include "materials/MeshMaterial.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

using namespace coral;

Model::Model(const std::string& path)
    : path(path)
{
    connect<&Model::init>(Object::init, this);
    connect<&Model::release>(Object::release, this);
}

void Model::init()
{
    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        Logs(error) << "Assimp error " << importer.GetErrorString();
        return;
    }
    // retrieve the directory path of the filepath
    directory = path.substr(0, path.find_last_of('/'));

    // process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene);

    // Clear data holder
    texturesByName.clear();
}

void Model::release()
{
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    // process each mesh located at the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene.
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        children.add(processMesh(mesh, scene));
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Handle<Mesh> Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    // walk through each of the mesh's vertices
    std::vector<Vertex> vertices;
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.

        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
        }

        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.textCoords = vec;
        }
        else
        {
            vertex.textCoords = glm::vec2(0.0f, 0.0f);
        }

        // tangent
        if (mesh->mTangents)
        {
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.tangent = vector;
        }

        // bitangent
        if (mesh->mBitangents)
        {
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.bitangent = vector;
        }

        vertices.push_back(vertex);
    }

    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    std::vector<unsigned int> indices;
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];

        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    // Get base color
    aiColor3D color(0, 0, 0);
    material->Get(AI_MATKEY_COLOR_DIFFUSE, color);

    // Get textures
    auto diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE);
    //auto specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR);
    //auto normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT);
    //auto heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT);

    // Create the mesh object
    std::string name = mesh->mName.C_Str();
    auto meshObject = ObjectFactory::createWithName<Mesh>(name, vertices, indices);

    // Create compatible material
    if (diffuseMaps.empty())
    {
        auto meshMaterial = ObjectFactory::createWithName<BasicMaterial>(name + "_material");
        meshMaterial->color = glm::vec3(color.r, color.g, color.g);
        meshMaterial->shininess = 10;
        meshObject->setMaterial(meshMaterial);
    }
    else
    {
        auto meshMaterial = ObjectFactory::createWithName<MeshMaterial>(name + "_material");
        meshMaterial->textureDiffuse = diffuseMaps[0];
        meshObject->setMaterial(meshMaterial);
    }

    return meshObject;
}

std::vector<Handle<Resource>> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type)
{
    std::vector<Handle<Resource>> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        // Get the name
        aiString str;
        mat->GetTexture(type, i, &str);
        std::string name = str.C_Str();

        // Does the resource already exists
        auto it = texturesByName.find(name);
        if (it != texturesByName.end())
        {
            textures.push_back(it->second);
        }
        // Create it
        else
        {
            std::string resourcePath = directory + "/" + name;
            Handle<Resource> texture = ObjectFactory::createWithName<Resource>(name, resourcePath);

            textures.push_back(texture);
            texturesByName[name] = texture;
        }
    }
    return textures;
}

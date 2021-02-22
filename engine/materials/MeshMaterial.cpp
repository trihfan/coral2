#include "MeshMaterial.h"
#include "scene/camera/Camera.h"
#include <glm/gtx/transform.hpp>
#include <unordered_map>

using namespace coral;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

static std::unordered_map<std::string, GLuint> sLoadedTextures;

MeshMaterial::MeshMaterial(const std::vector<Texture>& textures)
    : textures(textures)
{
    texture_ids.resize(textures.size(), 0);
    //shader = std::make_shared<Shader>("resources/shaders/1.model_loading.vs", "resources/shaders/1.model_loading.fs");

    // Load textures
    for (size_t i = 0; i < textures.size(); i++)
    {
        // already loaded
        auto it = sLoadedTextures.find(textures[i].path);
        if (it != sLoadedTextures.end())
        {
            texture_ids[i] = it->second;
        }
        // load
        else
        {
            //texture_ids[i] = textureFromFile(textures[i].path);
        }
    }
}

void MeshMaterial::use(const RenderParameters& parameters)
{
    // bind appropriate textures
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;

    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding

        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++); // transfer unsigned int to stream
        else if (name == "texture_normal")
            number = std::to_string(normalNr++); // transfer unsigned int to stream
        else if (name == "texture_height")
            number = std::to_string(heightNr++); // transfer unsigned int to stream

        // now set the sampler to the correct texture unit
        //glUniform1i(glGetUniformLocation(shader->getId(), (name + number).c_str()), i);

        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, texture_ids[i]);
    }

    // view/projection transformations
    /*glm::mat4 projection = glm::perspective(glm::radians(camera->zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera->getViewMatrix();
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);*/

    // render the loaded model
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f)); // it's a bit too big for our scene, so scale it down
    //shader->setMat4("model", model);
}

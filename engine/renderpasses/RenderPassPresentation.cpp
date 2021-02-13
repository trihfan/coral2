#include "RenderPassPresentation.h"
#include "Engine.h"
#include "materials/PresentationMaterial.h"
#include "scene/mesh/Mesh.h"
#include <numeric>

using namespace coral;

RenderPassPresentation::RenderPassPresentation()
{
    presentationMaterial = ObjectManager::create<PresentationMaterial>();

    std::vector<Vertex> vertices {
        Vertex { glm::vec3(-1, -1, 0), glm::vec3(0, 0, 1) },
        Vertex { glm::vec3(1, -1, 0), glm::vec3(0, 0, 1) },
        Vertex { glm::vec3(-1, 1, 0), glm::vec3(0, 0, 1) },
        Vertex { glm::vec3(-1, 1, 0), glm::vec3(0, 0, 1) },
        Vertex { glm::vec3(1, -1, 0), glm::vec3(0, 0, 1) },
        Vertex { glm::vec3(1, 1, 0), glm::vec3(0, 0, 1) },
    };

    std::vector<unsigned int> indices(vertices.size());
    std::iota(indices.begin(), indices.end(), 0);

    screenQuad = ObjectManager::create<Mesh>(vertices, indices);
    screenQuad->setMaterial(presentationMaterial);
}

void RenderPassPresentation::internalRender(RenderQueue& queue, const RenderParameters& parameters)
{
    glDisable(GL_DEPTH_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, 1);
    glBlitFramebuffer(0, 0, parameters.width, parameters.height, 0, 0, parameters.width, parameters.height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    return;

    auto resource = inputResources[0];

    presentationMaterial->getShader()->use();
    presentationMaterial->getShader()->setInt("backbuffer", 0);
    glActiveTexture(GL_TEXTURE0);
    resource->bind();

    screenQuad->draw(Engine::current_parameters);

    glEnable(GL_DEPTH_TEST);
    CHECK_OPENGL_ERROR
}
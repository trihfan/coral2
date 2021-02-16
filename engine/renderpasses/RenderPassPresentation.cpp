#include "RenderPassPresentation.h"
#include "Engine.h"
#include "ObjectFactory.h"
#include "materials/PresentationMaterial.h"
#include "resources/Resource.h"
#include "resources/Shader.h"
#include "scene/mesh/Mesh.h"
#include "utils/Error.h"
#include <numeric>

using namespace coral;

RenderPassPresentation::RenderPassPresentation()
{
    presentationMaterial = ObjectFactory::create<PresentationMaterial>();

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

    screenQuad = ObjectFactory::create<Mesh>(vertices, indices);
    screenQuad->setMaterial(presentationMaterial);
}

void RenderPassPresentation::internalRender(RenderQueue& queue, const RenderParameters& parameters)
{
    glDisable(GL_DEPTH_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glActiveTexture(GL_TEXTURE0);
    inputResources[0]->bind();

    presentationMaterial->getShader()->use();
    presentationMaterial->getShader()->setInt("backbuffer", 0);

    screenQuad->draw(parameters);

    glEnable(GL_DEPTH_TEST);
    CHECK_OPENGL_ERROR
}
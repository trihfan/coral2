#include "RenderPassPresentation.h"
#include "Engine.h"
#include "ObjectFactory.h"
#include "materials/PresentationMaterial.h"
#include "resources/Resource.h"
#include "scene/mesh/Mesh.h"
#include <numeric>

using namespace coral;

RenderPassPresentation::RenderPassPresentation()
{
    presentationMaterial = ObjectFactory::create<PresentationMaterial>();

    std::vector<Vertex> vertices {
        Vertex(glm::vec3(-1, -1, 0), glm::vec3(0, 0, 1)),
        Vertex(glm::vec3(1, 1, 0), glm::vec3(0, 0, 1)),
        Vertex(glm::vec3(1, -1, 0), glm::vec3(0, 0, 1)),

        Vertex(glm::vec3(-1, -1, 0), glm::vec3(0, 0, 1)),
        Vertex(glm::vec3(-1, 1, 0), glm::vec3(0, 0, 1)),
        Vertex(glm::vec3(1, 1, 0), glm::vec3(0, 0, 1)),
    };

    std::vector<unsigned int> indices(vertices.size());
    std::iota(indices.begin(), indices.end(), 0);

    screenQuad = ObjectFactory::create<Mesh>(vertices, indices);
    screenQuad->setMaterial(presentationMaterial);

    connect<&RenderPassPresentation::init>(Object::init, this);
    connect<&RenderPassPresentation::release>(Object::release, this);
}

void RenderPassPresentation::internalRender(RenderQueue&, const RenderParameters& parameters)
{
    defaultFramebuffer->bind(backend::BackendFramebufferUsage::write);

    inputResources[0]->bind();
    presentationMaterial->pipeline->use();
    presentationMaterial->use(parameters);

    screenQuad->draw(parameters);
}

void RenderPassPresentation::init()
{
    defaultFramebuffer = backend::BackendObjectFactory<backend::BackendDefaultFramebuffer>::create();
}

void RenderPassPresentation::release()
{
    defaultFramebuffer = nullptr;
}

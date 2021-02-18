#ifndef MESH_H
#define MESH_H

#include "scene/DrawableNode.h"
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

#include "backend/opengl/OpenGLBackend.h"

namespace coral
{
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texture_coordinates;
        glm::vec3 tangent;
        glm::vec3 bit_tangent;
    };

    class Mesh : public DrawableNode
    {
    public:
        // constructor
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

        // render the mesh;
        virtual void draw(const RenderParameters& parameters) override;

    private:
        // mesh Data
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        GLuint VAO;

        // render data
        GLuint VBO, EBO;

        // initializes all the buffer objects/arrays
        void setupMesh();

        // slots
        void init();
        void release();
    };
}
#endif

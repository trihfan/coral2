#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>
#include "Node.h"

namespace coral
{
    class Material;

    struct Vertex 
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texture_coordinates;
        glm::vec3 tangent;
        glm::vec3 bit_tangent;
    };

    class Mesh : public Node
    {
        DECLARE_TYPE(Mesh)
    public:
        // constructor
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, std::shared_ptr<Material> material);

        // render the mesh;
        virtual void draw(std::shared_ptr<coral::Camera> camera);

        // Shader material
        void setMaterial(std::shared_ptr<Material> material);
        std::shared_ptr<Material> getMaterial() const;

    private:
        // mesh Data
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        GLuint VAO;

        // render data 
        std::shared_ptr<Material> material;
        GLuint VBO, EBO;

        // initializes all the buffer objects/arrays
        void setupMesh();
    };
}
#endif
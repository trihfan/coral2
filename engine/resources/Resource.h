#ifndef RESOURCE_H
#define RESOURCE_H

#include <memory>
#include <unordered_map>
#include "Object.h"
#include "Property.h"
#include "utils/Singleton.h"
#include "glad/glad.h"

namespace coral
{
    class Engine;

    //
    class Resource : public Object
    {
    public:
        Resource();
  
        void bind();
        GLuint getId() const;

        // Properties
        Property<int> sampleCount;
        Property<int> width;
        Property<int> height;
        Property<GLint> internalFormat;
        Property<GLenum> format;
        Property<GLenum> type;

    private:
        // initialization
        void init();
        void release();
        
    private:
        GLuint id;
    };

    // The resource manager
    class ResourceManager
    {
        MAKE_ENGINE_SINGLETON(ResourceManager)

    public:
        static void clear();
        static std::shared_ptr<Resource> getResourceByName(const std::string& name);

        // Register named resources
        static void registerResource(std::shared_ptr<Resource> resource);

    private:
        ResourceManager(std::shared_ptr<std::pmr::memory_resource> memory_resource);

    private:
        std::unordered_map<std::string, std::shared_ptr<Resource>> resourceByName;
    };
}
#endif

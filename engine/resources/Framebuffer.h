#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <memory>
#include <unordered_map>
#include <vector>
#include "Object.h"
#include "utils/Singleton.h"
#include "glad/glad.h"

namespace coral
{
    class Engine;

    enum class ResourceRole { color, depth, stencil };

    //
    class Framebuffer : public Object
    {
    public:
        Framebuffer();

        // 
        void addResouce(const std::string& resource, ResourceRole type);
        const std::vector<std::pair<std::string, ResourceRole>>& getResouces() const;

        void bind(GLenum target = GL_DRAW_FRAMEBUFFER);
        
    private:
        // initialization
        void init();
        void release();
        
    private:
        GLuint framebufferId;
        std::vector<std::pair<std::string, ResourceRole>> resources;
    };

    // The framebuffer manager
    class FramebufferManager
    {
        MAKE_ENGINE_SINGLETON(FramebufferManager)
    public:
        static void clear();
        static std::shared_ptr<Framebuffer> getFramebufferFor(const std::vector<std::pair<std::string, ResourceRole>>& resources);

    private:
        FramebufferManager(std::shared_ptr<std::pmr::memory_resource> memory_resource);

    private:
        std::vector<std::shared_ptr<Framebuffer>> framebuffers;
    };
}
#endif

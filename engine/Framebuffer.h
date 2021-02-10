#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "Object.h"
#include "utils/Singleton.h"
#include "glad/glad.h"

namespace coral
{
    struct Resource
    {
        GLenum internalType;
        GLenum format;
        GLenum type;
    };

    //
    class Framebuffer : public Object
    {
    public:
        Framebuffer();

        // 
        void addResouce(const Resource& resouce);
        void setNumberOfSamples(int numberOfSamples);
  
        void bind(GLenum target = GL_DRAW_FRAMEBUFFER);
        
    private:
        // initialization
        void init();
        void release();
        
    private:
        GLuint framebufferId;
    };

    // The framebuffer manager
    class FramebufferManager
    {
        MAKE_ENGINE_SINGLETON(FramebufferManager)
    public:
        static std::shared_ptr<Framebuffer> getFramebuffer(const std::string& name);
        static std::shared_ptr<Framebuffer> getFramebufferFor(const std::vector<std::string>& colorOutputs);

    private:
        std::unordered_map<std::string, std::shared_ptr<Framebuffer>> frambufferByName;
    };
}
#endif

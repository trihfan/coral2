#include <numeric>
#include <cstddef>
#include <cstdint>
#include <fstream>
#define EGL_CAST(X, Y) static_cast<X>(Y)
#include "glad/glad_egl.h"
#include "Engine.h"
#include "Shader.h"
#include "Object.h"
#include "scene/Scene.h"
#include "scene/camera/Camera.h"
#include "scene/mesh/Mesh.h"
#include "materials/BasicMaterial.h"

using namespace coral;

// Egl parameters
static const EGLint configAttribs[]
{
    EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
    EGL_BLUE_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_RED_SIZE, 8,
    EGL_DEPTH_SIZE, 8,
    EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
    EGL_NONE
};    

static const int pbufferWidth = 800;
static const int pbufferHeight = 600;

static const EGLint pbufferAttribs[]
{
    EGL_WIDTH, pbufferWidth,
    EGL_HEIGHT, pbufferHeight,
    EGL_NONE,
};

// Bitmap info
struct BitmapHeader 
{
    char bitmapSignatureBytes[2] = {'B', 'M'};
    uint32_t sizeOfBitmapFile = 54 + 786432;
    uint32_t reservedBytes = 0;
    uint32_t pixelDataOffset = 54;
};

struct BmpInfoHeader 
{
    uint32_t sizeOfThisHeader = 40;
    int32_t width = pbufferWidth; // in pixels
    int32_t height = pbufferHeight; // in pixels
    uint16_t numberOfColorPlanes = 1; // must be 1
    uint16_t colorDepth = 24;
    uint32_t compressionMethod = 0;
    uint32_t rawBitmapDataSize = 0; // generally ignored
    int32_t horizontalResolution = 3780; // in pixel per meter
    int32_t verticalResolution = 3780; // in pixel per meter
    uint32_t colorTableEntries = 0;
    uint32_t importantColors = 0;
};

struct Pixel 
{
    uint8_t blue;
    uint8_t green;
    uint8_t red;
};

int main()
{
    // load egl
    // ------------------------------
    if (!gladLoadEGL())
    {
        Logs(error) << "Failed to initialize GLAD EGL";
        return;
    }

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    EGLint major, minor;
    eglInitialize(display, &major, &minor);
    Logs(info) << "Initialized EGL " << major << "." << minor;

    // 2. Select an appropriate configuration
    EGLint numConfigs;
    EGLConfig config;
    eglChooseConfig(display, configAttribs, &config, 1, &numConfigs);

    // 3. Create a surface
    EGLSurface surface = eglCreatePbufferSurface(display, config, pbufferAttribs);

    // 4. Bind the API
    eglBindAPI(EGL_OPENGL_API);

    // 5. Create a context and make it current
    EGLContext context = eglCreateContext(display, config, EGL_NO_CONTEXT, nullptr);
    eglMakeCurrent(display, surface, surface, context);

    // setup engine
    // ------------------------------
    ShaderManager::addShaderPath("resources/shaders");
    Engine::create();

    // scene
    auto scene = ObjectManager::createWithName<Scene>("scene");
    SceneManager::setCurrentScene(scene);

    // camera
    auto camera = ObjectManager::createWithName<Camera>("camera");
    camera->setPerspective(45.f, glm::vec4(0, 0, 800, 600), glm::vec2(0.1f, 100.f));
    camera->setView(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    camera->position = glm::vec3(0, 0, 3);
    scene->add(camera);

    // material
    auto material = ObjectManager::createWithName<BasicMaterial>("material");
    material->ambient = glm::vec3(0.1, 0.1, 0.1);
    material->diffuse = glm::vec3(0.1, 0.8, 0.3);
    material->specular = glm::vec3(0.8, 0.8, 0.8);
    material->shininess = 128;

    // vertices
    std::vector<Vertex> vertices 
    {
        Vertex{ glm::vec3(-0.5f, -0.5f, 0.f), glm::vec3(0.0f,  0.0f, 1.0f) },
        Vertex{ glm::vec3(0.5f,-0.5f, 0.f), glm::vec3(0.0f,  0.0f, 1.0f) },
        Vertex{ glm::vec3(-0.5f, 0.5f, 0.f), glm::vec3(0.0f,  0.0f, 1.0f) },
        Vertex{ glm::vec3(-0.5f, 0.5f, 0.f), glm::vec3(0.0f,  0.0f, 1.0f) },
        Vertex{ glm::vec3(0.5f, -0.5f, 0.f), glm::vec3(0.0f,  0.0f, 1.0f) },
        Vertex{ glm::vec3(0.5f, 0.5f, 0.f), glm::vec3(0.0f,  0.0f, 1.0f) },
    };

    std::vector<unsigned int> indices(vertices.size());
    std::iota(indices.begin(), indices.end(), 0);
    
    // mesh
    auto mesh = ObjectManager::createWithName<Mesh>("mesh", vertices, indices);
    mesh->setMaterial(material);
    scene->add(mesh);

    // main loop
    size_t numberOfPixels = pbufferWidth * pbufferHeight;
    uint8_t buffer[numberOfPixels * 4];
    while (true)
    {
        Engine::frame();

        // read pixels
        glReadPixels(0, 0, pbufferWidth, pbufferHeight, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, &buffer);

        // write bitmap
        std::ofstream fout("output.bmp", ios::binary);
        fout.write((char *) &bmpHeader, 14);
        fout.write((char *) &bmpInfoHeader, 40);

        // writing pixel data
        for (int i = 0; i < numberOfPixels; i++) 
        {
            Pixel pixel { buffer[i * 4], buffer[i * 4 + 1], buffer[i * 4 + 2] };
            fout.write((char *) &pixel, 3);
        }
        fout.close();

        eglSwapBuffers(display, surface);
        break;
    }

    Engine::destroy();
    eglTerminate(display);
}
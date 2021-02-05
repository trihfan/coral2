#include <numeric>
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

static const EGLint configAttribs[] = 
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

  static const EGLint pbufferAttribs[] = 
  {
    EGL_WIDTH, pbufferWidth,
    EGL_HEIGHT, pbufferHeight,
    EGL_NONE,
  };

int main()
{
    // load egl
    // ------------------------------
    if (!gladLoadEGL())
    {
        Logs(error) << "Failed to initialize GLAD EGL";
    }

    EGLDisplay eglDpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    EGLint major, minor;
    eglInitialize(eglDpy, &major, &minor);
    Logs(info) << "Initialized EGL " << major << "." << minor;

    // 2. Select an appropriate configuration
    EGLint numConfigs;
    EGLConfig eglCfg;
    eglChooseConfig(eglDpy, configAttribs, &eglCfg, 1, &numConfigs);

    // 3. Create a surface
    EGLSurface eglSurf = eglCreatePbufferSurface(eglDpy, eglCfg, pbufferAttribs);

    // 4. Bind the API
    eglBindAPI(EGL_OPENGL_API);

    // 5. Create a context and make it current
    EGLContext eglCtx = eglCreateContext(eglDpy, eglCfg, EGL_NO_CONTEXT, nullptr);
    eglMakeCurrent(eglDpy, eglSurf, eglSurf, eglCtx);

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
    // ------------------------------
    while (true)
    {
        Engine::frame();
        eglSwapBuffers(eglDpy, eglSurf);
    }

    Engine::destroy();
    eglTerminate(eglDpy);
}
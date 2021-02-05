#include <numeric>
#include <Windows.h>
#include <glad/glad.h>
#include "glad/glad_egl.h"
#include "glad/glad.h"
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
    EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
    EGL_BLUE_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_RED_SIZE, 8,
    EGL_ALPHA_SIZE, 8,
    EGL_DEPTH_SIZE, EGL_DONT_CARE,
    EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
    EGL_NONE
};    

static const int pbufferWidth = 800;
static const int pbufferHeight = 600;

static const EGLint windowsAttribs[]
{
    EGL_WIDTH, pbufferWidth,
    EGL_HEIGHT, pbufferHeight,
    EGL_NONE,
};

static const EGLint contextAttribs[]
{
    EGL_CONTEXT_MAJOR_VERSION, 3,
    EGL_CONTEXT_MINOR_VERSION, 3,
    EGL_CONTEXT_OPENGL_PROFILE_MASK, EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT,
    EGL_NONE,
};
			
static LRESULT CALLBACK __DummyWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
     return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int main()
{
    // create window
    WNDCLASS wc;
    memset(&wc, 0, sizeof(WNDCLASS));

    wc.lpfnWndProc   = __DummyWndProc;
    wc.hInstance     = GetModuleHandle(NULL);
    wc.lpszClassName = "DummyWindow";

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        "DummyWindow",
        "",
        0,
        0, 0, 800, 600,
        NULL,
        NULL,
		wc.hInstance,
        NULL
    );

    // load egl
    // ------------------------------
    if (!gladLoadEGL())
    {
        Logs(error) << "Failed to initialize GLAD EGL";
        return 1;
    }

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (display== EGL_NO_DISPLAY)
    {
        Logs(error) << "Can't get display";
        Logs(error) << std::hex << eglGetError();
        return 1;
    }

    EGLint major, minor;
    eglInitialize(display, &major, &minor);
    Logs(info) << "Initialized EGL " << major << "." << minor;

    // 2. Select an appropriate configuration
    EGLint numConfigs;
    EGLConfig config;
    if (!eglChooseConfig(display, configAttribs, &config, 1, &numConfigs) || numConfigs == 0)
    {
        Logs(error) << "Can't choose config";
         Logs(error) << std::hex << eglGetError();
        return 1;
    }

    // 3. Create a surface
    EGLSurface surface = eglCreateWindowSurface(display, config, hwnd, windowsAttribs);
    if (surface == EGL_NO_SURFACE)
    {
        Logs(error) << "Can't create surface";
        Logs(error) << std::hex << eglGetError();
        return 1;
    }

    // 4. Bind the API
    if (!eglBindAPI(EGL_OPENGL_API))
    {
        Logs(error) << "Can't bind api";
        Logs(error) << std::hex << eglGetError();
        return 1;
    }

    // 5. Create a context and make it current
    EGLContext context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
    if (context == EGL_NO_CONTEXT)
    {
        Logs(error) << "Can't create context";
        Logs(error) << std::hex << eglGetError();
        return 1;
    }

    if (!eglMakeCurrent(display, surface, surface, context))
    {
        Logs(error) << "Can't make the context current";
        Logs(error) << std::hex << eglGetError();
        return 1;
    }

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
    auto buffer = new unsigned char[numberOfPixels * 4];
    while (true)
    {
        Engine::frame();

        // read pixels
        glReadPixels(0, 0, pbufferWidth, pbufferHeight, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, buffer);

        // encode frame

        // send frame

        eglSwapBuffers(display, surface);
        break;
    }

    delete[] buffer;
    Engine::destroy();
    eglTerminate(display);
}
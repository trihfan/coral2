#include "Engine.h"
#include "OpenglBackend.h"
#include "base/Object.h"
#include "base/ObjectFactory.h"
//#include "VulkanBackend.h"
#ifndef __EMSCRIPTEN__
#include "glad/glad.h"
#endif
#include "scene/Scene.h"
#include "scene/SceneManager.h"
#include "scene/camera/OrbitCamera.h"
#include "scene/light/PointLight.h"
#include "scene/mesh/Animator.h"
#include "scene/mesh/Model.h"
#include "scene/text/Text.h"
#include <numeric>

// Glfw3 include
#ifdef EMSCRIPTEN
#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>
#include <emscripten.h>
#else
#include <GLFW/glfw3.h>
#endif

using namespace coral;
using namespace std::chrono;

// variables
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
static double lastX = SCR_WIDTH / 2.;
static double lastY = SCR_HEIGHT / 2.;
static bool firstMouse = true;
static bool mousePressed = false;
static GLFWwindow* window;

// Scene
static Handle<OrbitCamera> camera;
static std::vector<Handle<Model>> models;
static Handle<Text> text;
void setupScene();

// Animation
static std::vector<Handle<Animator>> animators;
static steady_clock::time_point start;
static double engineTime = 0;
static double animationStart = 0;
static size_t currentModel;
void advanceAnimation();
void restartAnimation();

// callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_button(GLFWwindow* window, int button, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

enum BackendType
{
    opengl,
    vulkan
};

void mainloop()
{
    // Get current time in seconds
    engineTime = static_cast<double>(duration_cast<microseconds>(steady_clock::now() - start).count()) / 1e6;

    // input
    processInput(window);

    // render
    Engine::frame();

    // Finish
    glfwSwapBuffers(window);
    glfwPollEvents();
}

int main()
{
    BackendType type = opengl;

    // glfw: initialize and configure
    // ------------------------------0
    glfwInit();
    if (type == opengl)
    {
#ifdef EMSCRIPTEN
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#else
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    }
    else if (type == vulkan)
    {
        if (!glfwVulkanSupported())
        {
            std::cout << "Vulkan not supported" << std::endl;
            return -1;
        }
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    }
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    // glfw window creation
    // --------------------
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "viewer", nullptr, nullptr);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    std::unique_ptr<backend::Backend> backend;
    if (type == opengl)
    {
        backend = std::make_unique<backend::opengl::OpenglBackend>();
    }
    else if (type == vulkan)
    {
        //backend = std::make_unique<backend::vulkan::VulkanBackend>(window);
        return 0;
    }

    // setup engine
    Engine::create(std::move(backend));
    Engine::resize(SCR_WIDTH, SCR_HEIGHT);
    setupScene();

    // main loop
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(mainloop, 0, true);
#else
    while (!glfwWindowShouldClose(window))
    {
        mainloop();
    }
#endif

    glfwTerminate();
    Engine::destroy();
    return 0;
}

void setupScene()
{
    // scene
    auto scene = ObjectFactory::createWithName<Scene>("scene");
    SceneManager::setCurrentScene(scene);

    // camera
    camera = ObjectFactory::createWithName<OrbitCamera>("camera");
    camera->setBackgroundColor(glm::vec4(1, 1, 1, 1));
    camera->setPerspective(45, glm::vec4(0, 0, SCR_WIDTH, SCR_HEIGHT), glm::vec2(0.1f, 100));
    camera->setView(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    camera->setTranslation(glm::vec3(0, 0, 3));
    camera->setDistanceMinMax(0.3f, 100);
    scene->add(camera);

    // Models
    models = {
        ObjectFactory::create<Model>("assets/models/Thriller Part 1.fbx"),
        ObjectFactory::create<Model>("assets/models/Thriller Part 2.fbx"),
        ObjectFactory::create<Model>("assets/models/Thriller Part 3.fbx"),
        ObjectFactory::create<Model>("assets/models/Thriller Part 4.fbx")
    };

    for (auto model : models)
    {
        model->setScale(glm::vec3(0.01, 0.01, 0.01));

        auto animator = ObjectFactory::create<Animator>("mixamo.com", model);
        connect<&advanceAnimation>(animator->animationFinished);
        animators.push_back(animator);

        scene->add(model);
        model->addChild(animator);
        model->setEnabled(false);
    }

    models[0]->setTranslation(glm::vec3(0, -1, 0));
    models[1]->setTranslation(glm::vec3(0, -1, 0));
    models[2]->setTranslation(glm::vec3(0, -1, 0));
    models[3]->setTranslation(glm::vec3(0, -1, 0));
    currentModel = 0;

    // Lights
    auto createLight = [scene](const glm::vec3& position) {
        auto light = ObjectFactory::create<PointLight>();
        light->setTranslation(position);
        light->color = glm::vec3(1, 1, 1);
        light->constant = 1;
        light->linear = 0.09f;
        light->quadratic = 0.032f;
        scene->add(light);
    };
    createLight(glm::vec3(5, 1, 5));
    createLight(glm::vec3(5, 1, -5));
    createLight(glm::vec3(-5, 1, -5));
    createLight(glm::vec3(-5, 1, 5));

    // Text
    TextFormat format;
    format.font = "assets/fonts/Ubuntu-C.ttf";
    format.size = 28;
    text = ObjectFactory::create<Text>(format);
    text->setColor(glm::vec3(0.1, 0.1, 0.1));
    text->setTranslation(glm::vec3(10, SCR_HEIGHT - 30, -1));
    text->setText("coral 0.1");
    scene->add(text);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void framebuffer_size_callback(GLFWwindow*, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    camera->setPerspective(45, glm::vec4(0, 0, width, height), glm::vec2(0.1f, 100));
    Engine::resize(width, height);

    text->setTranslation(glm::vec3(10, height - 30, 0));
}

void mouse_button(GLFWwindow*, int button, int action, int)
{
    if (button == 0)
    {
        mousePressed = action == GLFW_PRESS ? true : false;
        firstMouse = true;
    }
}

void mouse_callback(GLFWwindow*, double xpos, double ypos)
{
    if (!mousePressed)
    {
        return;
    }

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    double xoffset = xpos - lastX;
    double yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    // update camera.
    camera->move(static_cast<float>(xoffset) * 0.1f, static_cast<float>(yoffset) * 0.1f);
}

void scroll_callback(GLFWwindow*, double, double yoffset)
{
    camera->zoom(static_cast<float>(yoffset) * 0.3f);
}

void advanceAnimation()
{
    if (currentModel == models.size() - 1)
    {
        restartAnimation();
        return;
    }

    models[currentModel]->setEnabled(false);
    currentModel++;
    models[currentModel]->setEnabled(true);
    animators[currentModel]->restart();
}

void restartAnimation()
{
    models[currentModel]->setEnabled(false);
    currentModel = 0;
    models[currentModel]->setEnabled(true);
    animators[currentModel]->restart();
    animationStart = engineTime;
}

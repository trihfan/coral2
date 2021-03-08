#include "Engine.h"
#include "Object.h"
#include "ObjectFactory.h"
#include "OpenglBackend.h"
//#include "VulkanBackend.h"
#ifndef __EMSCRIPTEN__
#include "glad/glad.h"
#endif
#include "materials/BasicMaterial.h"
#include "scene/Scene.h"
#include "scene/SceneManager.h"
#include "scene/camera/OrbitCamera.h"
#include "scene/light/PointLight.h"
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

// variables
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
static double lastX = SCR_WIDTH / 2.;
static double lastY = SCR_HEIGHT / 2.;
static bool firstMouse = true;
static bool mousePressed = false;
static GLFWwindow* window;
static Handle<OrbitCamera> camera;

void setupScene();

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
    // input
    // -----
    processInput(window);

    // render
    // ------
    Engine::frame();

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
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
        /* backend = std::make_unique<backend::vulkan::VulkanBackend>(window);
        backend->init();
        backend->resize(SCR_WIDTH, SCR_HEIGHT);
        backend->destroy();*/
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
    camera->setDistanceMinMax(0.3f, 10);
    scene->add(camera);

    // model
    auto model = ObjectFactory::create<Model>("assets/models/droid/Droid.dae");
    model->setRotation(glm::vec3(-90, 0, 180));
    model->setTranslation(glm::vec3(0, -1, 0));
    scene->add(model);

    // Lights
    auto light1 = ObjectFactory::create<PointLight>();
    light1->setTranslation(glm::vec3(1, 1, 0.3));
    light1->color = glm::vec3(1, 1, 1);
    light1->constant = 1;
    light1->linear = 0.09f;
    light1->quadratic = 0.032f;
    scene->add(light1);

    auto light2 = ObjectFactory::create<PointLight>();
    light2->setTranslation(glm::vec3(-0.8, 1, 0.2));
    light2->color = glm::vec3(1, 1, 1);
    light2->constant = 1;
    light2->linear = 0.09f;
    light2->quadratic = 0.032f;
    scene->add(light2);

    auto light3 = ObjectFactory::create<PointLight>();
    light3->setTranslation(glm::vec3(-0.3, 1, -0.5));
    light3->color = glm::vec3(1, 1, 1);
    light3->constant = 1;
    light3->linear = 0.09f;
    light3->quadratic = 0.032f;
    scene->add(light3);

    // Text
    auto text = ObjectFactory::create<Text>("assets/fonts/Locanita.ttf");
    //text->setTranslation(glm::vec3(25, 25, 0));
    text->setScale(glm::vec3(0.004, 0.004, 0.004));
    text->setText("test");
    text->setColor(glm::vec3(0, 0.9, 0.4));
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
    Engine::resize(width * 2, height * 2);
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

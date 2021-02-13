#include "Engine.h"
#include "Object.h"
#include "Shader.h"
#include "glad/glad_egl.h"
#include "materials/BasicMaterial.h"
#include "nvencode/NvEncoderGL.h"
#include "nvencode/nvEncodeAPI.h"
#include "scene/Scene.h"
#include "scene/camera/Camera.h"
#include "scene/mesh/Mesh.h"
#include <QApplication>
#include <QDialog>
#include <QPainter>
#include <chrono>
#include <fstream>
#include <numeric>
#include <thread>
#include <vector>

using namespace coral;

// screen
int width = 800;
int height = 600;
bool running = true;

// Egl parameters
EGLDisplay display;
EGLSurface surface;
EGLContext context;

// encoder
NvEncoderGL* encoder;

// functions
bool initializeEgl();
bool setupEngine();
bool initializeEncoder();

// Window used to show debug output
class DebugOutput : public QDialog
{
public:
    DebugOutput()
    {
        buffer.resize(::width * ::height * 4);
        setFixedSize(::width, ::height);
        connect(this, &DebugOutput::finished, this, []() { running = false; });
    }

    void updateFrame()
    {
        glReadPixels(0, 0, ::width, ::height, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, buffer.data());
        update();
    }

    void paintEvent(QPaintEvent* event) override
    {
        QPainter painter(this);
        for (int x = 0; x < ::width; x++)
        {
            for (int y = 0; y < ::height; y++)
            {
                int current = y * ::width + x;
                painter.setPen(QColor(buffer[current * 4 + 1], buffer[current * 4 + 2], buffer[current * 4 + 3]));
                painter.drawPoint(x, y);
            }
        }
    }

private:
    std::vector<uint8_t> buffer;
};

int main(int argc, char** argv)
{
    // egl
    if (!initializeEgl())
    {
        return 1;
    }

    // engine
    if (!setupEngine())
    {
        return 1;
    }

    // encoder
    if (!initializeEncoder())
    {
        return 1;
    }

    // Create output debug window
    QApplication application(argc, argv);
    DebugOutput output;
    output.open();

    // Generate framebuffer
    GLuint framebuffer, depthbuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // depth
    {
        glGenTextures(1, &depthbuffer);
        glBindTexture(GL_TEXTURE_2D, depthbuffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthbuffer, 0);
    }

    // Create quad
    float vertices[] = {
        0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f
    };
    unsigned int indices[] = { 0, 1, 3, 1, 2, 3 };

    GLuint quadVbo, quadVao, quadEbo;

    glGenVertexArrays(1, &quadVao);
    glGenBuffers(1, &quadVbo);
    glGenBuffers(1, &quadEbo);
    glBindVertexArray(quadVao);

    glBindBuffer(GL_ARRAY_BUFFER, quadVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    auto shader = ShaderManager::getShader("encoder");

    // main loop
    std::vector<std::vector<uint8_t>> packet;
    std::ofstream fpOut("output.h264", std::ios::out | std::ios::binary);
    while (running)
    {
        auto start = std::chrono::steady_clock::now();

        // Render frame
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        Engine::frame();

        // show debug output
        output.updateFrame();
        application.processEvents();

        // get next buffer
        const NvEncInputFrame* encoderInputFrame = encoder->GetNextInputFrame();
        NV_ENC_INPUT_RESOURCE_OPENGL_TEX* resource = static_cast<NV_ENC_INPUT_RESOURCE_OPENGL_TEX*>(encoderInputFrame->inputPtr);

        // Convert frame
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glBindTexture(resource->target, resource->texture);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, resource->target, resource->texture, 0);
        glBindTexture(resource->target, 0);

        glDisable(GL_DEPTH_TEST);
        glClearColor(1, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        glViewport(0, 0, width * 4, height);
        shader->use();
        glBindVertexArray(quadVao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glEnable(GL_DEPTH_TEST);
        glViewport(0, 0, width, height);

        // encode frame
        encoder->EncodeFrame(packet);

        // send frame
        for (std::vector<uint8_t>& packet : packet)
        {
            fpOut.write(reinterpret_cast<char*>(packet.data()), packet.size());
        }

        // limit to 60 fps
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start).count();
        std::this_thread::sleep_for(std::chrono::microseconds(std::max(0l, 1000 * 16 - elapsed)));
    }
    encoder->EndEncode(packet);
    for (std::vector<uint8_t>& packet : packet)
    {
        fpOut.write(reinterpret_cast<char*>(packet.data()), packet.size());
    }

    delete encoder;
    Engine::destroy();
    eglTerminate(display);
    return 0;
}

bool initializeEgl()
{
    // load egl
    if (!gladLoadEGL())
    {
        Logs(error) << "Failed to initialize GLAD EGL";
        return false;
    }

    // get display
    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (display == EGL_NO_DISPLAY)
    {
        Logs(error) << "Can't get display (0x" << std::hex << eglGetError() << ")";
        return false;
    }

    // initialize egl
    EGLint major, minor;
    if (eglInitialize(display, &major, &minor) == EGL_FALSE)
    {
        Logs(error) << "Can't initialize egl (0x" << std::hex << eglGetError() << ")";
        return false;
    }

    // select an appropriate configuration
    const EGLint configAttribs[] {
        EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
        EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, EGL_DONT_CARE,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
        EGL_NONE
    };
    EGLint numConfigs;
    EGLConfig config;
    if (!eglChooseConfig(display, configAttribs, &config, 1, &numConfigs) || numConfigs == 0)
    {
        Logs(error) << "Can't choose config (0x" << std::hex << eglGetError() << ")";
        return false;
    }

    // create a surface
    const EGLint windowsAttribs[] {
        EGL_WIDTH,
        width,
        EGL_HEIGHT,
        height,
        EGL_NONE,
    };
    EGLSurface surface = eglCreatePbufferSurface(display, config, windowsAttribs);
    if (surface == EGL_NO_SURFACE)
    {
        Logs(error) << "Can't create surface (0x" << std::hex << eglGetError() << ")";
        return false;
    }

    // bind the api
    if (!eglBindAPI(EGL_OPENGL_API))
    {
        Logs(error) << "Can't bind api (0x" << std::hex << eglGetError() << ")";
        return false;
    }

    // create a context
    const EGLint contextAttribs[] {
        EGL_CONTEXT_MAJOR_VERSION,
        3,
        EGL_CONTEXT_MINOR_VERSION,
        3,
        EGL_CONTEXT_OPENGL_PROFILE_MASK,
        EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT,
        EGL_NONE,
    };
    EGLContext context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
    if (context == EGL_NO_CONTEXT)
    {
        Logs(error) << "Can't create context (0x" << std::hex << eglGetError() << ")";
        return false;
    }

    // make it current
    if (!eglMakeCurrent(display, surface, surface, context))
    {
        Logs(error) << "Can't make the context current (0x" << std::hex << eglGetError() << ")";
        return false;
    }

    Logs(info) << "Initialized EGL " << major << "." << minor;
    return true;
}

bool initializeEncoder()
{
    encoder = new NvEncoderGL(width, height, NV_ENC_BUFFER_FORMAT_ARGB);

    NV_ENC_INITIALIZE_PARAMS initializeParams = { NV_ENC_INITIALIZE_PARAMS_VER };
    NV_ENC_CONFIG encodeConfig = { NV_ENC_CONFIG_VER };
    initializeParams.encodeConfig = &encodeConfig;

    encoder->CreateDefaultEncoderParams(&initializeParams, NV_ENC_CODEC_H264_GUID, NV_ENC_PRESET_P7_GUID, NV_ENC_TUNING_INFO_ULTRA_LOW_LATENCY);
    encoder->CreateEncoder(&initializeParams);

    return true;
}

bool setupEngine()
{
    // setup engine
    ShaderManager::addShaderPath("resources/shaders");
    ShaderManager::addShaderPath(".");
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
    std::vector<Vertex> vertices {
        Vertex { glm::vec3(-0.5f, -0.5f, 0.f), glm::vec3(0.0f, 0.0f, 1.0f) },
        Vertex { glm::vec3(0.5f, -0.5f, 0.f), glm::vec3(0.0f, 0.0f, 1.0f) },
        Vertex { glm::vec3(-0.5f, 0.5f, 0.f), glm::vec3(0.0f, 0.0f, 1.0f) },
        Vertex { glm::vec3(-0.5f, 0.5f, 0.f), glm::vec3(0.0f, 0.0f, 1.0f) },
        Vertex { glm::vec3(0.5f, -0.5f, 0.f), glm::vec3(0.0f, 0.0f, 1.0f) },
        Vertex { glm::vec3(0.5f, 0.5f, 0.f), glm::vec3(0.0f, 0.0f, 1.0f) },
    };

    std::vector<unsigned int> indices(vertices.size());
    std::iota(indices.begin(), indices.end(), 0);

    // mesh
    auto mesh = ObjectManager::createWithName<Mesh>("mesh", vertices, indices);
    mesh->setMaterial(material);
    scene->add(mesh);
    return true;
}

static inline bool operator==(const GUID& guid1, const GUID& guid2)
{
    return !memcmp(&guid1, &guid2, sizeof(GUID));
}

static inline bool operator!=(const GUID& guid1, const GUID& guid2)
{
    return !(guid1 == guid2);
}

/*bool initializeEncoder()
{
    // create instance
    NVENCSTATUS result = NvEncodeAPIcreate(&encoderInstance);
    if (result != NVENCSTATUS::NV_ENC_SUCCESS)
    {
        Logs(error) << "Failed to create encode api instance (" << result << ")";
        return false;
    }

    // encoding session
    NV_ENC_OPEN_ENCODE_SESSION_EX_PARAMS encodeSessionExParams = { NV_ENC_OPEN_ENCODE_SESSION_EX_PARAMS_VER };
    encodeSessionExParams.device = nullptr;
    encodeSessionExParams.deviceType = NV_ENC_DEVICE_TYPE_OPENGL;
    encodeSessionExParams.apiVersion = NVENCAPI_VERSION;
    
    result = encoderInstance.nvEncOpenEncodeSessionEx(&encodeSessionExParams, &encoder);
    if (result != NVENCSTATUS::NV_ENC_SUCCESS)
    {
        Logs(error) << "Failed to create encode session (" << result << ")";
        return false;
    }

    // list guid
    uint32_t guidCount;
    result = encoderInstance.nvEncGetEncodeGUIDCount(encoder, &guidCount);
    if (result != NVENCSTATUS::NV_ENC_SUCCESS)
    {
        Logs(error) << "Failed to get guid count (" << result << ")";
        return false;
    }

    std::vector<GUID> guids(guidCount);
    result = encoderInstance.nvEncGetEncodeGUIDs(encoder, guids.data(), guidCount, &guidCount);
    if (result != NVENCSTATUS::NV_ENC_SUCCESS)
    {
        Logs(error) << "Failed to get guids( " << result << ")";
        return false;
    }

    // select encode guid
    bool found = false;
    for (const GUID& guid : guids)
    {
        if (guid == NV_ENC_CODEC_H264_GUID)
        {
            encodeGuid = guid;
            found = true;
            break;
        }
    }

    if (!found)
    {
        Logs(error) << "Can't found h264 guid";
        return false;
    }

    // list preset
    result = encoderInstance.nvEncGetEncodePresetCount(encoder, encodeGuid, &guidCount);
    if (result != NVENCSTATUS::NV_ENC_SUCCESS)
    {
        Logs(error) << "Failed to get preset count (" << result << ")";
        return false;
    }

    guids.resize(guidCount);
    result = encoderInstance.nvEncGetEncodePresetGUIDs(encoder, encodeGuid, guids.data(), guidCount, &guidCount);
    if (result != NVENCSTATUS::NV_ENC_SUCCESS)
    {
        Logs(error) << "Failed to get preset (" << result << ")";
        return false;
    }

    // select preset
    found = false;
    for (const GUID& guid : guids)
    {
        if (guid == NV_ENC_PRESET_P7_GUID)
        {
            presetGuid = guid;
            found = true;
            break;
        }
    }

    if (!found)
    {
        Logs(error) << "Can't found preset guid";
        return false;
    }

    NV_ENC_PRESET_CONFIG presetConfig { NV_ENC_PRESET_CONFIG_VER, { NV_ENC_CONFIG_VER } };
    result = encoderInstance.nvEncGetEncodePresetConfigEx(encoder, encodeGuid, presetGuid, NV_ENC_TUNING_INFO::NV_ENC_TUNING_INFO_ULTRA_LOW_LATENCY, &presetConfig);
    if (result != NVENCSTATUS::NV_ENC_SUCCESS)
    {
        Logs(error) << "Can't set low latency config (" << result << ")";
        return false;
    }
    return true;

    // list formats
    uint32_t inputTypeCount;
    result = encoderInstance.nvEncGetInputFormatCount(encoder, encodeGuid, &inputTypeCount);
    if (result != NVENCSTATUS::NV_ENC_SUCCESS)
    {
        Logs(error) << "Failed to get input format count (" << result << ")";
        return false;
    }

    std::vector<NV_ENC_BUFFER_FORMAT> formats(inputTypeCount);
    result = encoderInstance.nvEncGetInputFormats(encoder, encodeGuid, formats.data(), inputTypeCount, &inputTypeCount);
    if (result != NVENCSTATUS::NV_ENC_SUCCESS)
    {
        Logs(error) << "Failed to get formats ( " << result << ")";
        return false;
    }

    // select format
    found = false;
    for (const NV_ENC_BUFFER_FORMAT& format : formats)
    {
        if (format == NV_ENC_BUFFER_FORMAT_ARGB)
        {
            found = true;
            break;
        }
    }

    if (!found)
    {
        Logs(error) << "Can't found format";
        return false;
    }

    // initialize encoder session
    NV_ENC_INITIALIZE_PARAMS initializedParams {};
    initializedParams.encodeConfig->version = NV_ENC_CONFIG_VER;
    initializedParams.version = NV_ENC_INITIALIZE_PARAMS_VER;

    initializedParams.encodeGUID = encodeGuid;
    initializedParams.presetGUID = presetGuid;
    initializedParams.encodeWidth = width;
    initializedParams.encodeHeight = height;
    initializedParams.darWidth = width;
    initializedParams.darHeight = height;
    initializedParams.frameRateNum = 30;
    initializedParams.frameRateDen = 1;
    initializedParams.enablePTD = 1;
    initializedParams.reportSliceOffsets = 0;
    initializedParams.enableSubFrameWrite = 0;
    initializedParams.maxEncodeWidth = width;
    initializedParams.maxEncodeHeight = height;
    initializedParams.enableMEOnlyMode = false;
    initializedParams.enableOutputInVidmem = false;

    initializedParams.encodeConfig->frameIntervalP = 1;
    initializedParams.encodeConfig->gopLength = NVENC_INFINITE_GOPLENGTH;
    initializedParams.encodeConfig->rcParams.rateControlMode = NV_ENC_PARAMS_RC_CBR;
    initializedParams.encodeConfig->encodeCodecConfig.h264Config.idrPeriod = initializedParams.encodeConfig->gopLength;

    result = encoderInstance.nvEncInitializeEncoder(encoder, &initializedParams);
    if (result != NVENCSTATUS::NV_ENC_SUCCESS)
    {
        Logs(error) << "Can't initialize hardware session ( " << result << ")";
        return false;
    }

    // Create input

    // Create output

    return true;
}*/
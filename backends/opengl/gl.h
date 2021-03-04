#pragma once

#ifdef __EMSCRIPTEN__
#define GLFW_INCLUDE_ES3
#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>

#include <GLES2/gl2ext.h>
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

cmake_minimum_required(VERSION 3.16)

# Unity build
if(NOT EMSCRIPTEN)
    set(CMAKE_UNITY_BUILD ON)
endif()

# Project
set(version "1.0.0")
project(coral VERSION ${version} LANGUAGES CXX)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Set c++17
set(CMAKE_CXX_STANDARD 17)
if(UNIX AND NOT APPLE)
        set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -lstdc++ -std=c++17")
endif()

# Modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
include(folder_structure)

# Dependencies
find_package(OpenGL)
find_package(Vulkan)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

# Options
option(BACKEND_OPENGL_ENABLED "enable opengl backend" ${OPENGL_FOUND})
option(BACKEND_VULKAN_ENABLED "enable vulkan backend" ${VULKAN_FOUND})

# Definitions
if(BACKEND_OPENGL_ENABLED)
    add_definitions(-DENABLE_OPENGL)
endif()
if(BACKEND_VULKAN_ENABLED)
    add_definitions(-DENABLE_VULKAN)
endif()

# define output directories
set(lib_output_directory ${PROJECT_BINARY_DIR}/lib)
set(bin_output_directory ${PROJECT_BINARY_DIR}/bin)

# Emscripten config
if(EMSCRIPTEN)
    add_compile_options("-s" "USE_FREETYPE=1")
    set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -s USE_GLFW=3 -s USE_WEBGL2=1 -s FULL_ES3=1 -s WASM=1 -s USE_FREETYPE=1 -s -s USE_ZLIB=1 --preload-file ${bin_output_directory}/assets@/assets")
endif()

# subdirectories
add_subdirectory(backends) # Properties: backend_interfaces_path, backend_opengl_path, backend_vulkan_path
add_subdirectory(engine) # Properties: engine_path
add_subdirectory(utils) # Properties: utils_path
add_subdirectory(tests)
add_subdirectory(assets_packager)

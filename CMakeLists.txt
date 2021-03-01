cmake_minimum_required(VERSION 3.16)

# unity build
if(NOT EMSCRIPTEN)
    #set(CMAKE_UNITY_BUILD ON)
endif()

# project
set(version "1.0.0")
project(coral VERSION ${version} LANGUAGES CXX)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# use c++17
set(CMAKE_CXX_STANDARD 17)
if(UNIX AND NOT APPLE)
        set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -lstdc++ -std=c++17")
endif()

# modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
include(folder_structure)

# Python 3
find_package(Python3 REQUIRED)

# Options
find_package(OpenGL)
find_package(Vulkan)
option(ENABLE_OPENGL "enable opengl backend" ${OPENGL_FOUND})
option(ENABLE_VULKAN "enable vulkan backend" ${VULKAN_FOUND})

if(ENABLE_OPENGL)
    add_definitions(-DENABLE_OPENGL)
endif()
if(ENABLE_VULKAN)
    add_definitions(-DENABLE_VULKAN)
endif()

# Emscripten
if(EMSCRIPTEN)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
    set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -s USE_GLFW=3 -s USE_WEBGL2=1 -s FULL_ES3=1 -s WASM=1 -s USE_PTHREADS=1")
endif()

# define output directories
set(lib_output_directory ${PROJECT_BINARY_DIR}/lib)
set(bin_output_directory ${PROJECT_BINARY_DIR}/bin)

# directories
set(glm_path ${PROJECT_SOURCE_DIR}/glm)
set(backends_path ${PROJECT_SOURCE_DIR}/backends)
set(engine_path ${PROJECT_SOURCE_DIR}/engine)
set(utils_path ${PROJECT_SOURCE_DIR}/utils)
set(test_path ${PROJECT_SOURCE_DIR}/tests)

# subdirectories
add_subdirectory(glm)
add_subdirectory(backends)
add_subdirectory(engine)
add_subdirectory(utils)
add_subdirectory(tests)

# resources
add_subdirectory(resources)

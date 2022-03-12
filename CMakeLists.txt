cmake_minimum_required(VERSION 3.16)

# Unity build
set(CMAKE_UNITY_BUILD ON)

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
find_package(Vulkan)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

# define output directories
set(lib_output_directory ${PROJECT_BINARY_DIR}/lib)
set(bin_output_directory ${PROJECT_BINARY_DIR}/bin)

# subdirectories
add_subdirectory(engine)
add_subdirectory(demos)

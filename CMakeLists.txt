cmake_minimum_required(VERSION 3.16)

# unity build
set(CMAKE_UNITY_BUILD ON)

# project
set(version "1.0.0")
project(coral VERSION ${version} LANGUAGES CXX)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# use c++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -std=c++17")

if(UNIX AND NOT APPLE)
        set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -lstdc++")
endif()

# modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
include(folder_structure)

# Python 3
find_package(Python3 REQUIRED)

# define output directories
set(lib_output_directory ${PROJECT_BINARY_DIR}/lib)
set(bin_output_directory ${PROJECT_BINARY_DIR}/bin)

# directories
set(glm_path ${PROJECT_SOURCE_DIR}/glm)
set(engine_path ${PROJECT_SOURCE_DIR}/engine)
set(test_path ${PROJECT_SOURCE_DIR}/test)

# subdirectories
add_subdirectory(glm)
add_subdirectory(engine)
add_subdirectory(tests)

# resources
add_subdirectory(resources)
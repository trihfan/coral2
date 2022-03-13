cmake_minimum_required(VERSION 3.16)
project(coral)
set(CMAKE_CXX_STANDARD 17)

# Download and include conan cmake
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.16.1/conan.cmake" "${CMAKE_BINARY_DIR}/conan.cmake"
         EXPECTED_HASH SHA256=396e16d0f5eabdc6a14afddbcfff62a54a7ee75c6da23f32f7a31bc85db23484 TLS_VERIFY ON)
endif()
include(${CMAKE_BINARY_DIR}/conan.cmake)

# Install dependencies
conan_cmake_install(PATH_OR_REFERENCE ${CMAKE_SOURCE_DIR} GENERATOR cmake_find_package)

# Add bin directory to search
list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

# Dependencies
find_package(VulkanHeaders REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)
find_package(freetype REQUIRED)
find_package(glfw3 REQUIRED)
find_package(boost REQUIRED)

# Modules
add_subdirectory(engine)
add_subdirectory(demos)

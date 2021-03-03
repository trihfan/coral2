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
option(BACKEND_OPENGL_ENABLED "enable opengl backend" ${OPENGL_FOUND})
option(BACKEND_VULKAN_ENABLED "enable vulkan backend" ${VULKAN_FOUND})

if(BACKEND_OPENGL_ENABLED)
    add_definitions(-DENABLE_OPENGL)
endif()
if(BACKEND_VULKAN_ENABLED)
    add_definitions(-DENABLE_VULKAN)
endif()

# define output directories
set(lib_output_directory ${PROJECT_BINARY_DIR}/lib)
set(bin_output_directory ${PROJECT_BINARY_DIR}/bin)

# directories
set(glm_path ${PROJECT_SOURCE_DIR}/glm)
set(backends_path ${PROJECT_SOURCE_DIR}/backends)
set(engine_path ${PROJECT_SOURCE_DIR}/engine)
set(utils_path ${PROJECT_SOURCE_DIR}/utils)
set(assets_path ${PROJECT_SOURCE_DIR}/assets)
set(test_path ${PROJECT_SOURCE_DIR}/tests)

# Emscripten config
if(EMSCRIPTEN)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
    set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -s USE_GLFW=3 -s USE_WEBGL2=1 -s FULL_ES3=1 -s WASM=1 -s USE_PTHREADS=1 --preload-file ${assets_path}@/assets")
endif()

# subdirectories
add_subdirectory(glm)
add_subdirectory(backends)
add_subdirectory(engine)
add_subdirectory(utils)
add_subdirectory(tests)

# Asset package
set(asset_binary_dir "${bin_output_directory}/${CMAKE_CFG_INTDIR}/assets")
set(ASSET_DIRECTORIES "${assets_path}")

file(WRITE ${CMAKE_BINARY_DIR}/assets.h "")
file(APPEND ${CMAKE_BINARY_DIR}/assets.h "#define ASSETS_LIST std::vector<Asset>{\\\n")

file(MAKE_DIRECTORY ${bin_output_directory})
foreach(ASSET_DIRECTORY ${ASSET_DIRECTORIES})

    # List all assets recursively
    file(GLOB_RECURSE asset_list ${ASSET_DIRECTORY}/*)

    # For each asset
    foreach(asset_file ${asset_list})

        # Copy to binary dir
        file(RELATIVE_PATH asset_file_relative ${ASSET_DIRECTORY} ${asset_file})
        configure_file(${asset_file} "${asset_binary_dir}/${asset_file_relative}" COPYONLY)


        # Package in asset.h
        get_filename_component(asset_file_name ${asset_file} NAME)
        file(APPEND ${CMAKE_BINARY_DIR}/assets.h "Asset{\"${asset_file_name}\", \"assets/${asset_file_relative}\"},\\\n")

    endforeach()
endforeach()

file(APPEND ${CMAKE_BINARY_DIR}/assets.h "}")

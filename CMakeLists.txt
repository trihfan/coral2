cmake_minimum_required(VERSION 3.16)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(version "1.0.0")
project(coral VERSION ${version} LANGUAGES CXX)

# use c++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -std=c++17")

if(UNIX AND NOT APPLE)
        set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -lstdc++")
endif()


# modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
include(folder_structure)

# packages
find_package(Python3 REQUIRED)

# directories
set(glm_path ${PROJECT_SOURCE_DIR}/glm)
set(engine_path ${PROJECT_SOURCE_DIR}/engine)
set(test_path ${PROJECT_SOURCE_DIR}/test)

# define output directories
set(lib_output_directory ${PROJECT_BINARY_DIR}/lib)
set(bin_output_directory ${PROJECT_BINARY_DIR}/bin)

# subdirectories
add_subdirectory(glm)
add_subdirectory(engine)
add_subdirectory(tests)

# resources
file(WRITE ${PROJECT_SOURCE_DIR}/last_copy.h "")
add_custom_target(copy_resources ALL 
				  DEPENDS ${PROJECT_SOURCE_DIR}/last_copy.h)
add_custom_command(TARGET copy_resources PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E make_directory "${bin_output_directory}/${CMAKE_CFG_INTDIR}/resources")
add_custom_command(TARGET copy_resources PRE_BUILD
                   COMMAND ${Python3_EXECUTABLE} copy.py "${PROJECT_SOURCE_DIR}/resources" "${bin_output_directory}/${CMAKE_CFG_INTDIR}/resources"
				   WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
add_dependencies(engine copy_resources)

# setup coral project directory
set_target_properties(copy_resources PROPERTIES FOLDER "coral/utils")
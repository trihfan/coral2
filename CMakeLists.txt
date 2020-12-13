cmake_minimum_required(VERSION 3.12)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(version "1.0.0")
project(coral VERSION ${version} LANGUAGES CXX)

# use c++17
set(CMAKE_CXX_STANDARD 17)

# modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)
include(folder_structure)

# directories
set(external_path ${PROJECT_SOURCE_DIR}/externals)
set(engine_path ${PROJECT_SOURCE_DIR}/engine)
set(viewer_path ${PROJECT_SOURCE_DIR}/tests/viewer)
set(assimp_config_path ${PROJECT_BINARY_DIR}/externals/assimp/include)

# define output directories
set(lib_output_directory ${PROJECT_BINARY_DIR}/lib)
set(bin_output_directory ${PROJECT_BINARY_DIR}/bin)

# externals
add_subdirectory(externals)
add_subdirectory(engine)
add_subdirectory(tests)

# resources
add_custom_target(copy_resources ALL 
				  DEPENDS ${PROJECT_SOURCE_DIR}/last_copy.h)
add_custom_command(TARGET copy_resources PRE_BUILD
                   COMMAND python copy.py "${PROJECT_SOURCE_DIR}/resources" "${bin_output_directory}/${CMAKE_CFG_INTDIR}/resources"
				   WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
add_dependencies(engine copy_resources)
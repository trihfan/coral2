project(engine)
set_property(GLOBAL PROPERTY engine_path ${CMAKE_CURRENT_SOURCE_DIR})

# Dependencies
get_property(glm_path GLOBAL PROPERTY glm_path)
get_property(utils_path GLOBAL PROPERTY utils_path)
get_property(backend_interfaces_path GLOBAL PROPERTY backend_interfaces_path)
get_property(assimp_path GLOBAL PROPERTY assimp_path)

if(NOT EMSCRIPTEN)
    find_package(Freetype REQUIRED)
endif()

# Definitions
add_definitions(-DSTB_IMAGE_STATIC -DSTB_IMAGE_IMPLEMENTATION -DAI_MAX_BONE_WEIGHTS=12)

# Sources
file(GLOB_RECURSE sources *.h *.cpp *.c)

# Make lib
add_library(engine STATIC ${sources})
set_folder_structure("${sources}" ${CMAKE_CURRENT_SOURCE_DIR})

# Includes
target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${utils_path} ${backend_interfaces_path} ${glm_path} ${assimp_path} ${FREETYPE_INCLUDE_DIRS})

# Libraries
target_link_libraries(engine PUBLIC utils backend_interfaces glm assimp)

if(NOT EMSCRIPTEN)
    target_link_libraries(engine PUBLIC Freetype::Freetype)
endif()

# Set output directories
set_target_properties(engine PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(engine PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(engine PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_output_directory})
set_target_properties(engine PROPERTIES FOLDER "coral")


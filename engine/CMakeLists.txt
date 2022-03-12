project(engine)
set_property(GLOBAL PROPERTY engine_path ${CMAKE_CURRENT_SOURCE_DIR})

# Dependencies
find_package(freetype CONFIG REQUIRED)
find_package(Vulkan REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

# Definitions
add_definitions(-DSTB_IMAGE_STATIC -DSTB_IMAGE_IMPLEMENTATION -DAI_MAX_BONE_WEIGHTS=12)
add_definitions(-DGLFW_INCLUDE_VULKAN)
option(VULKAN_DEBUG "use vulkan validation layers" OFF)
if (VULKAN_DEBUG)
	add_definitions(-DUSE_VALIDATION_LAYERS)
endif()

# Sources
file(GLOB_RECURSE sources *.h *.cpp *.c)

# Make lib
add_library(engine SHARED  ${sources})
set_folder_structure("${sources}" ${CMAKE_CURRENT_SOURCE_DIR})

# Includes
target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${utils_path} ${backend_interfaces_path})

# Libraries
target_link_libraries(engine PUBLIC Vulkan::Vulkan glfw assimp::assimp glm::glm freetype)

# Set output directories
set_target_properties(engine PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(engine PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(engine PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_output_directory})
set_target_properties(engine PROPERTIES FOLDER "coral")


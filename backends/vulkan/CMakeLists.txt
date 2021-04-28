project(backend_vulkan)
set_property(GLOBAL PROPERTY backend_vulkan_path ${CMAKE_CURRENT_SOURCE_DIR})

# Dependencies
get_property(glm_path GLOBAL PROPERTY glm_path)
get_property(utils_path GLOBAL PROPERTY utils_path)
get_property(backend_interfaces_path GLOBAL PROPERTY backend_interfaces_path)
find_package(Vulkan REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

# Definitions
add_definitions(-DGLFW_INCLUDE_VULKAN)
option(BACKEND_VULKAN_DEBUG "use vulkan validation layers" OFF)
if (BACKEND_VULKAN_DEBUG)
	add_definitions(-DUSE_VALIDATION_LAYERS)
endif()

# Sources
file(GLOB_RECURSE sources *.h *.cpp *.c)

# Make lib
add_library(backend_vulkan STATIC ${sources})
target_include_directories(backend_vulkan PUBLIC "${backend_vulkan_path}" "${utils_path}" "${backend_interfaces_path}" "${glm_path}")
target_link_libraries(backend_vulkan PUBLIC Vulkan::Vulkan glfw utils)
set_folder_structure("${sources}" ${CMAKE_CURRENT_SOURCE_DIR})

# Set output directories
set_target_properties(backend_vulkan PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(backend_vulkan PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(backend_vulkan PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_output_directory})
set_target_properties(backend_vulkan PROPERTIES FOLDER "backends")

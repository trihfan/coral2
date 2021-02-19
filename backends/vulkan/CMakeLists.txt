project(vulkan_backend)

# Sources
file(GLOB_RECURSE sources *.h *.cpp *.c)
set_folder_structure("${sources}" ${backend_vulkan_path})

# Make lib
add_library(vulkan_backend STATIC ${sources})
target_include_directories(vulkan_backend PUBLIC "${backend_vulkan_path}")
target_include_directories(vulkan_backend PUBLIC "${utils_path}")
target_link_libraries(vulkan_backend PUBLIC utils)

# Interfaces
target_include_directories(vulkan_backend PUBLIC "${backend_interfaces_path}")

# Vulkan
find_package(Vulkan REQUIRED)

find_package(glfw3 CONFIG REQUIRED)
add_definitions(-DGLFW_INCLUDE_VULKAN)

option(USE_VALIDATION_LAYERS "use vulkan validation layers" OFF)
if (USE_VALIDATION_LAYERS)
	add_definitions(-DUSE_VALIDATION_LAYERS)
endif()

target_link_libraries(vulkan_backend PUBLIC Vulkan::Vulkan)
target_link_libraries(vulkan_backend PUBLIC glfw)

# glm
target_include_directories(vulkan_backend PUBLIC "${glm_path}")
add_dependencies(vulkan_backend glm)

# Set output directories
set_target_properties(vulkan_backend PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(vulkan_backend PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(vulkan_backend PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_output_directory})
set_target_properties(vulkan_backend PROPERTIES FOLDER "backends")

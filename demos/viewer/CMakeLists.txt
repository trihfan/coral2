project(viewer)

# Definitions
add_definitions(-DSTB_IMAGE_STATIC -DSTB_IMAGE_IMPLEMENTATION -DAI_MAX_BONE_WEIGHTS=12)
add_definitions(-DGLFW_INCLUDE_VULKAN)
option(VULKAN_DEBUG "use vulkan validation layers" OFF)
if (VULKAN_DEBUG)
        add_definitions(-DUSE_VALIDATION_LAYERS)
endif()

# Sources
file(GLOB_RECURSE sources *.h *.cpp)

# Make exe
add_executable(viewer ${sources})

# Includes
target_include_directories(viewer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Libraries
target_link_libraries(viewer PRIVATE engine)

project(backends)

# Interface backend
add_subdirectory(interfaces)

# OpenGL
if(BACKEND_OPENGL_ENABLED)
    add_subdirectory(opengl)
endif()

# Vulkan
if(BACKEND_VULKAN_ENABLED)
    #add_subdirectory(vulkan)
endif()

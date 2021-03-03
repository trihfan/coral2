project(backends)

# directories
set(backend_interfaces_path ${backends_path}/interfaces)
set(backend_opengl_path ${backends_path}/opengl)
set(backend_vulkan_path ${backends_path}/vulkan)

add_subdirectory(interfaces)

# OpenGL
if(BACKEND_OPENGL_ENABLED)
    add_subdirectory(opengl)
endif()

# Vulkan
if(BACKEND_VULKAN_ENABLED)
    add_subdirectory(vulkan)
endif()

project (tests)

# externals
find_package(OpenGL COMPONENTS OpenGL EGL)

set(viewer_path ${PROJECT_SOURCE_DIR}/tests/viewer)
set(signals_path ${PROJECT_SOURCE_DIR}/tests/signals)
set(web_path ${PROJECT_SOURCE_DIR}/tests/web)

add_subdirectory(viewer)
add_subdirectory(signals)

if (OpenGL_EGL_FOUND)
    add_subdirectory(web)
endif()
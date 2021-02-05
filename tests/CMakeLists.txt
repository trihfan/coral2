project(tests)

# externals
find_package(OpenGL COMPONENTS OpenGL EGL)

set(viewer_path ${PROJECT_SOURCE_DIR}/tests/viewer)
set(signals_path ${PROJECT_SOURCE_DIR}/tests/signals)
set(web_path ${PROJECT_SOURCE_DIR}/tests/web)
set(emscripten_path ${PROJECT_SOURCE_DIR}/tests/emscripten)

if(EMSCRIPTEN)
    add_subdirectory(emscripten)   
else()
    add_subdirectory(viewer)
    add_subdirectory(signals)
endif()

if(OpenGL_EGL_FOUND)
    add_subdirectory(web)
endif()
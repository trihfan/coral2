project(tests)

# directories
set(viewer_path ${PROJECT_SOURCE_DIR}/tests/viewer)
set(signals_path ${PROJECT_SOURCE_DIR}/tests/signals)
set(web_path ${PROJECT_SOURCE_DIR}/tests/web)
set(emscripten_viewer_path ${PROJECT_SOURCE_DIR}/tests/emscripten-viewer)

# emscripten
if(EMSCRIPTEN)
    add_subdirectory(emscripten-viewer)
else()
# default
    add_subdirectory(viewer)
    add_subdirectory(signals)

    # server side render on linux
    if(NOT APPLE)
        add_subdirectory(web)
    endif()
endif()


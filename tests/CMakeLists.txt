project(tests)

# directories
set(viewer_path ${test_path}/viewer)
set(signals_path ${test_path}/signals)
set(web_path ${test_path}/web)
set(emscripten_viewer_path ${test_path}/emscripten-viewer)

# emscripten
if(EMSCRIPTEN)
    add_subdirectory(emscripten-viewer)
else()
# default
    add_subdirectory(viewer)
    add_subdirectory(signals)

    # server side render on linux
    if(NOT APPLE AND UNIX)
        add_subdirectory(web)
    endif()
endif()


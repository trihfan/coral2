project(tests)

# directories
set(viewer_path ${PROJECT_SOURCE_DIR}/tests/viewer)
set(signals_path ${PROJECT_SOURCE_DIR}/tests/signals)
set(web_path ${PROJECT_SOURCE_DIR}/tests/web)
set(emscripten_path ${PROJECT_SOURCE_DIR}/tests/emscripten)

# emscripten
if(EMSCRIPTEN)
    add_subdirectory(emscripten)   
else()
# default
    add_subdirectory(viewer)
    add_subdirectory(signals)
endif()

# server side render on linux
if(UNIX AND NOT APPLE)
    add_subdirectory(web)
endif()
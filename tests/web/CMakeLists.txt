project (web)

# EGL
find_package(OpenGL REQUIRED COMPONENTS OpenGL EGL)
if(WIN32)
    include_directories(web PRIVATE "${OPENGL_EGL_INCLUDE_DIRS}")
    link_directories(web ${OPENGL_egl_LIBRARY})
endif()

# nvidia encode
set(NVENCODEAPI_INCLUDE_DIR "" CACHE STRING "Set nvidia encode sdk include directory")
set(NVENCODER_INCLUDE_DIR "" CACHE STRING "Set nvidia encoder codec include directory")
include_directories(web PRIVATE "${NVENCODEAPI_INCLUDE_DIR}" "${NVENCODER_INCLUDE_DIR}")

# make lib
link_directories(web ${lib_output_directory})
file(GLOB_RECURSE sources *.h *.cpp *.c)
set_folder_structure("${sources}" ${web_path})
add_executable(web ${sources})

# dependencies
target_link_libraries(web engine)
if(WIN32)
    target_link_libraries(web egl)
else()
    target_link_libraries(web OpengGL::EGL)
endif()

# set output directories
set_target_properties(web PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(web PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(web PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_output_directory})
set_target_properties(viewer PROPERTIES FOLDER "coral/web")
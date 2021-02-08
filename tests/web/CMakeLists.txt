project (web)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# opengl egl
find_package(OpenGL REQUIRED COMPONENTS OpenGL EGL)

# nvidia encode
find_library(NVENCODEAPI_LIB nvidia-encode)

# qt for debug output
find_package(Qt6 COMPONENTS Core Widgets REQUIRED)

# make lib
link_directories(web ${lib_output_directory})
file(GLOB_RECURSE sources *.h *.cpp *.c)
set_folder_structure("${sources}" ${web_path})
add_executable(web ${sources})

# dependencies
target_link_libraries(web PRIVATE engine OpenGL::EGL ${NVENCODEAPI_LIB} Qt6::Core Qt6::Widgets)

# set output directories
set_target_properties(web PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(web PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(web PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_output_directory})
set_target_properties(viewer PROPERTIES FOLDER "coral/web")

# resource

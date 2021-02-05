project (viewer)

# externals
find_package(glfw3 CONFIG REQUIRED)

# link directories
link_directories(viewer ${lib_output_directory})

# make lib
file(GLOB_RECURSE sources *.h *.cpp *.inl)
set_folder_structure("${sources}" ${viewer_path})
add_executable(viewer ${sources})

# includes
target_include_directories(viewer PUBLIC "${external_path}/glfw/include")

# dependencies
target_link_libraries(viewer engine glfw)

# set output directories
set_target_properties(viewer PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(viewer PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(viewer PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_output_directory})
set_target_properties(viewer PROPERTIES FOLDER "coral/tests")
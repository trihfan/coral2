project (viewer)

# include directories
include_directories("${engine_path}")
include_directories("${external_path}/glad/include")
include_directories("${external_path}/glfw/include")
include_directories("${external_path}/glm")

# link directories
link_directories(viewer ${lib_output_directory})

# make lib
file(GLOB_RECURSE sources *.h *.cpp *.inl)
set_folder_structure("${sources}" ${viewer_path})
add_executable(viewer ${sources})
add_dependencies(viewer engine)

target_link_libraries(viewer engine)
target_link_libraries(viewer glfw)

# set output directories
set_target_properties(viewer PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(viewer PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(viewer PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_output_directory})

# copy ressources
file(COPY "${viewer_path}/resources" DESTINATION ${bin_output_directory})
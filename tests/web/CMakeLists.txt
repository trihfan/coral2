project (web)

# link directories
link_directories(web ${lib_output_directory})

# make lib
file(GLOB_RECURSE sources *.h *.cpp *.c)
set_folder_structure("${sources}" ${web_path})
add_executable(web ${sources})

# includes
target_include_directories(web PRIVATE "${web_path}")

# dependencies
target_link_libraries(web engine egl)

# set output directories
set_target_properties(web PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(web PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(web PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_output_directory})
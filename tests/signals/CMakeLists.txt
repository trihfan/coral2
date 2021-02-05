project (signals)

# link directories
link_directories(signals ${lib_output_directory})

# make lib
file(GLOB_RECURSE sources *.h *.cpp)
set_folder_structure("${sources}" ${signals_path})
add_executable(signals ${sources})

# dependencies
target_link_libraries(signals engine)

# set output directories
set_target_properties(signals PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(signals PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(signals PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_output_directory})
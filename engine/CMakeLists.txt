project(engine)

# externals
find_package(OpenGL REQUIRED)

# link directories
link_directories(engine ${lib_output_directory})

# make lib
file(GLOB_RECURSE sources *.h *.cpp *.c)
set_folder_structure("${sources}" ${engine_path})
add_library(engine STATIC ${sources})
#target_precompile_headers(engine PRIVATE pch.h)

# includes
target_include_directories(engine PUBLIC "${engine_path}")
target_include_directories(engine PUBLIC "${glm_path}")

# dependencies
target_link_libraries(engine PUBLIC ${CMAKE_DL_LIBS})

if(EMSCRIPTEN)
    target_link_libraries(engine PUBLIC OPENGL_gl_LIBRARY)    
else()
    target_link_libraries(engine PUBLIC OpenGL::GL)   
endif()

add_dependencies(engine glm)

# set output directories
set_target_properties(engine PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(engine PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(engine PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_output_directory})
set_target_properties(engine PROPERTIES FOLDER "coral")

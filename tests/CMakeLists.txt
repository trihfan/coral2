project(tests)

# directories
set(viewer_path ${test_path}/viewer)
set(signals_path ${test_path}/signals)
set(web_path ${test_path}/web)

add_subdirectory(viewer)
add_subdirectory(signals)
#add_subdirectory(web)

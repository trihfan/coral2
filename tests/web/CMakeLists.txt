project(web)

# directories
set(server_path ${web_path}/server)
set(client_path ${web_path}/client)

add_subdirectory(server)
add_subdirectory(client)


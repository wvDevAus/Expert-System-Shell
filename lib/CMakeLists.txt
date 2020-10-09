# Insert the libraries into the CMake Tree
add_subdirectory(json)

# Disable the JSON libary's automated tests
set(JSON_BuildTests OFF CACHE INTERNAL "")

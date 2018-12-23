cmake_minimum_required (VERSION 2.6)

include_directories(dep/glm)
include_directories(dep/glew-2.1.0/include)
include_directories(dep/glfw-3.2.1/include)
include_directories(dep/assimp-4.1.0/include)
include_directories(inc)

add_subdirectory(dep)
add_subdirectory(src)

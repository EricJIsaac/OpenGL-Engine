set(PROJECT_NAME "opengl-engine")

project(${PROJECT_NAME})

find_library(GL REQUIRED)
find_library(GLEW REQUIRED)
find_library(glfw REQUIRED)
find_library(assimp REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)
add_subdirectory(engine)
add_subdirectory(graphics)

target_link_libraries(${PROJECT_NAME}
  graphics
  engine
  GLEW
  glfw
  assimp
  GL
)

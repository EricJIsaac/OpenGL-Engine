set(PROJECT_NAME "opengl-engine")

project(${PROJECT_NAME})

find_library(GL REQUIRED)
find_library(GLEW REQUIRED)
find_library(glfw REQUIRED)
find_library(assimp REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)
add_subdirectory(engine)
add_subdirectory(graphics)

target_link_libraries(${PROJECT_NAME} GL)
target_link_libraries(${PROJECT_NAME} GLEW)
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} assimp)

target_link_libraries(${PROJECT_NAME} graphics)
target_link_libraries(${PROJECT_NAME} engine)

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                 ${CMAKE_SOURCE_DIR}/data ${CMAKE_BINARY_DIR}/src/data)

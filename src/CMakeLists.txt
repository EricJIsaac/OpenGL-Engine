set(PROJECT_NAME "opengl-engine")

project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(graphics)
add_subdirectory(engine)

add_dependencies(${PROJECT_NAME} GL)
add_dependencies(${PROJECT_NAME} glew)
add_dependencies(${PROJECT_NAME} glfw)
add_dependencies(${PROJECT_NAME} assimp)

target_link_libraries(${PROJECT_NAME} graphics)
target_link_libraries(${PROJECT_NAME} engine)

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                 ${CMAKE_SOURCE_DIR}/data ${CMAKE_BINARY_DIR}/src/data)

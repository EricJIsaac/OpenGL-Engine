add_executable(${PROJECT_NAME} main.cpp)

if(WIN32)
  message("Building for windows")

  find_package(OpenGL REQUIRED)
  find_package(GLEW CONFIG REQUIRED)
  find_package(glfw3 CONFIG REQUIRED)
  find_package(assimp CONFIG REQUIRED)
  find_package(glm CONFIG REQUIRED)

  include_directories(
    "${GLEW_INCLUDE_DIR}"
  )

  set(OpenGL_LIBS
    GLEW::GLEW
    glfw
    glm
    assimp::assimp
    "${OPENGL_LIBRARIES}"
  )
    
  file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR}/bin/Debug)
  file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR}/bin/Release)
endif()

if(UNIX AND NOT APPLE)
  find_library(GL REQUIRED)
  find_library(GLEW REQUIRED)
  find_library(glfw REQUIRED)
  find_library(assimp REQUIRED)

  set(OpenGL_LIBS
    GLEW
    glfw
    assimp
    GL
  )
  
  file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR}/bin)
endif()

add_subdirectory(engine)
add_subdirectory(graphics)

target_link_libraries(${PROJECT_NAME}
  graphics
  engine
  ${OpenGL_LIBS}
)
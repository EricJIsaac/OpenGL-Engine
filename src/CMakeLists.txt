add_executable(${PROJECT_NAME} main.cpp)

if(WIN32)
  message("Building for windows")
  find_package(OpenGL REQUIRED)

  set(OpenGL_LIBS
    "${CMAKE_SOURCE_DIR}/lib/win32/glew32.lib"
    "${CMAKE_SOURCE_DIR}/lib/win32/glfw3dll.lib"
    "${CMAKE_SOURCE_DIR}/lib/win32/assimp32.lib"
    "${OPENGL_LIBRARIES}"
  )

  include_directories(
    "${CMAKE_SOURCE_DIR}/lib/win32/include"
  )
    
  file(COPY ${CMAKE_SOURCE_DIR}/lib/win32/glew32.dll DESTINATION ${CMAKE_BINARY_DIR}/bin/Debug)
  file(COPY ${CMAKE_SOURCE_DIR}/lib/win32/glew32.dll DESTINATION ${CMAKE_BINARY_DIR}/bin/Release)
  file(COPY ${CMAKE_SOURCE_DIR}/lib/win32/glfw3.dll DESTINATION ${CMAKE_BINARY_DIR}/bin/Debug)
  file(COPY ${CMAKE_SOURCE_DIR}/lib/win32/glfw3.dll DESTINATION ${CMAKE_BINARY_DIR}/bin/Release)
  file(COPY ${CMAKE_SOURCE_DIR}/lib/win32/assimp32.dll DESTINATION ${CMAKE_BINARY_DIR}/bin/Debug)
  file(COPY ${CMAKE_SOURCE_DIR}/lib/win32/assimp32.dll DESTINATION ${CMAKE_BINARY_DIR}/bin/Release)  
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
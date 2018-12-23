set(SOURCES
  "camera_script.cpp"
  "input_map.cpp"
  "../graphics/transform.cpp"
  "../graphics/node.cpp"
  "../graphics/camera_component.cpp"
)

add_library(engine STATIC ${SOURCES})

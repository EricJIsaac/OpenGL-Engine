set(SOURCES
  "anim_node.cpp"
  "animation.cpp"
  "camera_component.cpp"
  "mesh_buffer.cpp"
  "mesh.cpp"
  "node.cpp"
  "shader.cpp"
  "skeleton_mesh_buffer.cpp"
  "skeleton_mesh.cpp"
  "skeleton.cpp"
  "transform.cpp"
)

add_library(graphics STATIC ${SOURCES})

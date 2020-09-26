set(SOURCES
  "jsoncpp.cpp"
  "anim_node.cpp"
  "animation.cpp"
  "camera_component.cpp"
  "mesh_buffer.cpp"
  "mesh.cpp"
  "mesh_db.cpp"
  "node.cpp"
  "render.cpp"
  "save_image.cpp"
  "scene.cpp"
  "shader.cpp"
  "skeleton_mesh_buffer.cpp"
  "skeleton_mesh.cpp"
  "skeleton.cpp"
  "scene.cpp"
  "transform.cpp"
)

add_library(graphics STATIC ${SOURCES})

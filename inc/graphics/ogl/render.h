#ifndef GRAPHICS_RENDER_H
#define GRAPHICS_RENDER_H

#include "graphics/ogl/shader.h"
#include "graphics/ogl/mesh_buffer.h"
#include "graphics/ogl/skeleton_mesh_buffer.h"

namespace graphics {
namespace scene {
  struct Node;
} // namespace scene

namespace ogl {

  void render(
    graphics::scene::Node* root,
    graphics::ogl::MeshBuffer& mb,
    glm::mat4 start,
    const graphics::ogl::Shader& shader);

  void render(
    graphics::scene::Node* root,
    graphics::ogl::SkeletonMeshBuffer& mb,
    glm::mat4 start,
    const graphics::ogl::Shader& shader);

} // namespace ogl
} // namespace graphics

#endif // GRAPHICS_RENDER_H

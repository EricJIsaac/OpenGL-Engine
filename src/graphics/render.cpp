#include "graphics/ogl/render.h"
#include "graphics/scene/node.h"
#include "graphics/scene/mesh_component.h"
#include "graphics/scene/skeleton_mesh_component.h"

namespace graphics {
namespace ogl {

  void render(
    graphics::scene::Node* root,
    graphics::ogl::MeshBuffer& mb,
    glm::mat4 start,
    const graphics::ogl::Shader& shader)
  {
    if(root == nullptr)
    {
      return;
    }

    auto meshes = root->getComponents(
      graphics::scene::component::Type::Mesh);

    glm::mat4 mvp = start * root->getTransform().getMatrix();
    glUniformMatrix4fv(shader.uniform("mvp"), 1, GL_FALSE, &mvp[0][0]);

    for(std::size_t i = 0; i < meshes.size(); ++i)
    {
      auto mc = (graphics::scene::MeshComponent*)meshes[i];
      mb.draw(mc->getId());
    }

    for(std::size_t i = 0; i < root->numChildren(); ++i)
    {
      render(root->getChild(i), mb, mvp, shader);
    }
  }

  void render(
    graphics::scene::Node* root,
    graphics::ogl::SkeletonMeshBuffer& mb,
    glm::mat4 start,
    const graphics::ogl::Shader& shader)
  {
    if(root == nullptr)
    {
      return;
    }

    auto meshes = root->getComponents(
      graphics::scene::component::Type::SkeletonMesh);

    glm::mat4 mvp = start * root->getTransform().getMatrix();
    glUniformMatrix4fv(shader.uniform("mvp"), 1, GL_FALSE, &mvp[0][0]);

    for(std::size_t i = 0; i < meshes.size(); ++i)
    {
      auto smc = (graphics::scene::SkeletonMeshComponent*)meshes[i];
      if(!smc->isAnimating())
      {
        continue;
      }

      std::vector<glm::mat4x4> bpos;
      mb.getBonePositions(smc->getId(),
  			smc->getAnimName(),
  			smc->getAnimTime(),
  			bpos);

      glUniformMatrix4fv(shader.uniform("bpos"), 50, GL_FALSE, &bpos.at(0)[0][0]);
      mb.draw(smc->getId());
    }

    for(std::size_t i = 0; i < root->numChildren(); ++i)
    {
      render(root->getChild(i), mb, mvp, shader);
    }
  }

} // namespace ogl
} // namespace graphics

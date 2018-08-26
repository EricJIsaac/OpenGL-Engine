#include "graphics/data/skeleton.h"

namespace graphics {
namespace data {

void Skeleton::getMatrices(
  std::string anim_name,
  float anim_time,
  matrix4x4_map& out)
{
  // anim_transforms
  this->animations[anim_name].getMatrices(anim_time, out);
  evaluateOffsets(this->root, out, glm::mat4(1.0f));
}

void Skeleton::evaluateOffsets(
  BoneNode& root,
  matrix4x4_map& anim_matrices,
  glm::mat4 parent_offset
)
{
  glm::mat4 anim_mat = glm::mat4(1.0f);
  if(anim_matrices.count(root.bone_name) > 0)
  {
    anim_mat = anim_matrices[root.bone_name];
  }

  glm::mat4 global_transform = parent_offset * anim_mat;
  anim_matrices[root.bone_name] =
    this->inv_global_transform *
    global_transform *
    root.bone_offset;

  for(std::size_t i = 0; i < root.children.size(); ++i)
  {
    this->evaluateOffsets(
      root.children[i],
      anim_matrices,
      global_transform
    );
  }
}

} // namespace data
} // namespace graphics

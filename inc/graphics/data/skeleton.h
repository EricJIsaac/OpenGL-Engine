#ifndef GRAPHICS_SKELETON_H
#define GRAPHICS_SKELETON_H

#include <string>
#include <vector>
#include <unordered_map>
#include <glm/mat4x4.hpp>
#include "graphics/data/bone_node.h"
#include "graphics/data/animation.h"
#include "graphics/data/data_types.h"

namespace graphics {
namespace data {

struct Skeleton {
public:
  BoneNode root;
  animation_map animations;
  std::vector<std::string> bone_names;
  glm::mat4 inv_global_transform;
  
  void getMatrices(
    std::string anim_name,
    float anim_time,
    matrix4x4_map& out
  );

private:
  void evaluateOffsets(
    BoneNode& root,
    matrix4x4_map& anim_matrices,
    glm::mat4 parent_offset
  );
};

} // namespace data
} // namespace graphics

#endif // GRAPHICS_SKELETON_H

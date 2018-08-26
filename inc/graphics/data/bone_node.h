#ifndef GRAPHICS_BONE_NODE_H
#define GRAPHICS_BONE_NODE_H

#include <string>
#include <glm/mat4x4.hpp>

namespace graphics {
namespace data {

struct BoneNode {
  std::string bone_name;
  glm::mat4 bone_offset;

  std::vector<BoneNode> children;
};

} // namespace data
} // namespace graphics

#endif // GRAPHICS_BONE_NODE_H

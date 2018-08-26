#ifndef GRAPHICS_DATA_TYPES_H
#define GRAPHICS_DATA_TYPES_H

#include <unordered_map>
#include <glm/mat4x4.hpp>

namespace graphics {
namespace data {

class Animation;

using animation_map =
  std::unordered_map<std::string,Animation>;

using matrix4x4_map =
  std::unordered_map<std::string, glm::mat4>;

} // namespace data
} // namespace graphics

#endif // GRAPHICS_DATA_TYPES_H

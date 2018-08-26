#ifndef GRAPHICS_KEYFRAME_H
#define GRAPHICS_KEYFRAME_H

#include <glm/vec3.hpp>

namespace graphics {
namespace data {

struct Keyframe {
public:
  glm::vec3 vec;
  float time;
};

} // namespace data
} // namespace graphics

#endif // GRAPHICS_KEYFRAME_H

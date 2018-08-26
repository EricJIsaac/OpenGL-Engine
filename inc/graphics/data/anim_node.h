#ifndef GRAPHICS_ANIM_NODE_H
#define GRAPHICS_ANIM_NODE_H

#include <string>
#include <vector>
#include "graphics/data/keyframe.h"
#include "graphics/scene/transform.h"

namespace graphics {
namespace data {
namespace anim {
enum Behavior {
  FirstFrame = 1,
  LoopFrames,
  LastFrame
};
} // namespace anim

struct AnimNode {
public:
  std::string bone_name;
  std::vector<Keyframe> positions;
  std::vector<Keyframe> scales;
  std::vector<Keyframe> rotations;

  void setAnimBehavior(anim::Behavior b);
  void setAnimationTime(float t);
  void sortKeyFrames();
  void getTransform(
    float anim_time,
    graphics::scene::Transform& outTransform);

private:
  void interpolate(
    std::vector<Keyframe>& vec,
    float anim_time,
    glm::vec3& out);

  anim::Behavior m_animBehavior;
  float m_animation_end_time;
};

} // namespace data
} // namespace graphics

#endif // GRAPHICS_ANIM_NODE_H

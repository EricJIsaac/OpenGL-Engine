#ifndef GRAPHICS_ANIMATION_H
#define GRAPHICS_ANIMATION_H

#include "graphics/data/anim_node.h"
#include "graphics/data/data_types.h"
#include <string>
#include <vector>
#include <unordered_map>

namespace graphics {
namespace data {

struct Animation
{
public:
  std::string anim_name;
  std::vector<AnimNode> anim_nodes;

  void setAnimBehavior(anim::Behavior b);
  void setEndTime(float endTime);
  const float getEndTime() const;
  void getMatrices(float anim_time, matrix4x4_map& out);

private:
  float m_endTime;
  anim::Behavior m_animBehavior;
};

} // namespace data
} // namespace graphics

#endif // GRAPHICS_ANIMATION_H

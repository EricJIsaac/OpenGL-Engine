#include "graphics/data/animation.h"

namespace graphics {
namespace data {

void Animation::setAnimBehavior(anim::Behavior b)
{
  for(std::size_t i = 0; i < this->anim_nodes.size(); ++i)
  {
    this->anim_nodes[i].setAnimBehavior(b);
  }

  this->m_animBehavior = b;
}

void Animation::setEndTime(float endTime)
{
  for(std::size_t i = 0; i < this->anim_nodes.size(); ++i)
  {
    this->anim_nodes[i].setAnimationTime(endTime);
  }

  this->m_endTime = endTime;
}

const float Animation::getEndTime() const
{
  return this->m_endTime;
}

void Animation::getMatrices(
  float anim_time, matrix4x4_map& out)
{
  for(std::size_t i = 0; i < this->anim_nodes.size(); ++i)
  {
    graphics::scene::Transform transform;
    this->anim_nodes[i].getTransform(anim_time, transform);
    out[this->anim_nodes[i].bone_name] = transform.getMatrix();
  }
}

} // namespace data
} // namespace graphics

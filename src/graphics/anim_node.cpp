#include "graphics/data/anim_node.h"
#include <cmath>
#include <cstdio>
#include <vector>
#include <algorithm>

namespace graphics {
namespace data {

void AnimNode::setAnimBehavior(anim::Behavior b)
{
  this->m_animBehavior = b;
}

void AnimNode::setAnimationTime(float t)
{
  this->m_animation_end_time = t;
}

void AnimNode::sortKeyFrames()
{
  std::sort(positions.begin(), positions.end(),
  [](Keyframe a, Keyframe b) {
      return a.time > b.time;
  });

  std::sort(scales.begin(), scales.end(),
  [](Keyframe a, Keyframe b) {
      return a.time > b.time;
  });

  std::sort(rotations.begin(), rotations.end(),
  [](Keyframe a, Keyframe b) {
      return a.time > b.time;
  });
}

void AnimNode::getTransform(
  float anim_time, graphics::scene::Transform& outTransform)
{
  glm::vec3 vec;
  this->interpolate(
    this->positions, anim_time, vec);
  outTransform.setTranslation(vec);

  this->interpolate(
    this->scales, anim_time, vec);
  outTransform.setScale(vec);

  this->interpolate(
    this->rotations, anim_time, vec);
  outTransform.setRotation(vec);
}

void AnimNode::interpolate(
  std::vector<Keyframe>& vec,
  float anim_time,
  glm::vec3& out)
{
  if(vec.size() == 0)
  {
    out = glm::vec3();
    return;
  }

  if(vec.size() == 1)
  {
    out = vec[0].vec;
    return;
  }

  if(anim_time > this->m_animation_end_time)
  {
    switch(this->m_animBehavior)
    {
      case anim::Behavior::FirstFrame:
      {
        out = vec[0].vec;
        return;
      } break;
      case anim::Behavior::LastFrame:
      {
        out = vec[vec.size() - 1].vec;
        return;
      } break;
      case anim::Behavior::LoopFrames:
      {
        anim_time =
          anim_time -
          m_animation_end_time * std::floor(anim_time / m_animation_end_time);
      } break;
      default:
      {
        printf("Unrecognized anim behavior %i in anim node.\n",
        (int)this->m_animBehavior);
        return;
      } break;
    }
  }

  // Assuming less than 9999 keyframes
  std::size_t index = 9999;
  for(std::size_t i = 0; i < vec.size(); ++i)
  {
    if(anim_time <= vec[i].time)
    {
      index = i;
      break;
    }
  }

  if(index == 9999)
  {
    // No valid animation time found
    out = glm::vec3();
    return;
  }

  if(index == 0)
  {
    out = vec[0].vec;
    return;
  }
    
  float tlength = vec[index].time - vec[index-1].time;
  float interp = (anim_time-vec[index-1].time) / tlength;
  out = vec[index-1].vec * (1 - interp) + interp * vec[index].vec;
  return;
}

} // namespace data
} // namespace graphics

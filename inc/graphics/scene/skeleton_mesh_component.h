#ifndef GRAPHICS_SKELETON_MESH_COMPONENT_H
#define GRAPHICS_SKELETON_MESH_COMPONENT_H

#include "graphics/scene/component.h"

namespace graphics {
namespace scene {

class SkeletonMeshComponent : public Component
{
public:
  SkeletonMeshComponent()
  {
    this->m_type = component::Type::SkeletonMesh;
  }

  void setId(unsigned int id)
  { this->m_meshId = id; }

  void setAnimName(std::string s)
  { this->m_animName = s; }

  void setAnimTime(float f)
  { this->m_animTime = f; }
  
  void updateAnimTime(float dt)
  { this->m_animTime += dt; }

  const bool isAnimating() const
  { return m_isAnimating; }

  const unsigned int getId() const
  { return this->m_meshId; }

  const std::string getAnimName() const
  { return this->m_animName; }

  const float getAnimTime() const
  { return m_animTime; }

private:
  unsigned int m_meshId;
  float m_animTime = 0.0f;
  std::string m_animName = "";
  bool m_isAnimating = true;
};

 
}
}

#endif // GRAPHCIS_SKELETON_MESH_COMPONENT_H

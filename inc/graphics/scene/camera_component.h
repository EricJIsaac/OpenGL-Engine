#ifndef GRAPHICS_CAMERA_COMPONENT_H
#define GRAPHICS_CAMERA_COMPONENT_H

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "graphics/scene/component.h"

namespace graphics {
namespace scene {

class CameraComponent : public Component
{
public:
  CameraComponent();

  glm::mat4 getViewMatrix();  
  glm::mat4 getProjectionMatrix();

  void setTarget(glm::vec3 target);
 
private:
  glm::vec3  m_target;
  
  float m_fov = 45.f;
  float m_aspect = 4.f / 3.f;
  float m_near = 0.1f;
  float m_far = 100.f;
};
  
} // namespace scene
} // namespace graphics
  

#endif // GRAPHICS_CAMERA_COMPONENT_H

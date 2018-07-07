#include "graphics/scene/camera_component.h"
#include "graphics/scene/node.h"

namespace graphics {
namespace scene {

  CameraComponent::CameraComponent()
  {
    this->m_type = component::Type::Camera;
  }
  
  glm::mat4 CameraComponent::getViewMatrix()
    {
      glm::vec4 up = glm::vec4(0, 1, 0, 1);
      glm::vec4 position = glm::vec4(0,0,0,1);
      if(this->m_owner != nullptr)
	{
	  position = this->m_owner->getTransform().getAbsoluteMatrix() * position;
	  up = this->m_owner->getTransform().getAbsoluteMatrix() * up;
	}
      
      return glm::lookAt( glm::vec3(position), this->m_target, glm::vec3(up));
    }
  
  glm::mat4 CameraComponent::getProjectionMatrix()
    {
      return glm::perspective
	(glm::radians(this->m_fov),
	 this->m_aspect,
	 this->m_near,
	 this->m_far);
    }

  void CameraComponent::setTarget(glm::vec3 target)
  {
    this->m_target = target;
  }
  
} // namespace scene
} // namespace graphics

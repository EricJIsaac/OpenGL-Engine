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
      glm::vec4 up = glm::vec4(0, 1, 0, 0);
      glm::vec4 position = glm::vec4(0,0,0,1);
      glm::vec4 dir = glm::vec4(1,0,0,0);
      if(this->m_owner != nullptr)
    	{
        auto abs_transform = this->m_owner->getTransform().getAbsoluteMatrix();
    	  position = abs_transform * position;
    	  up = abs_transform * up;
        dir = abs_transform * dir;
    	}

      return glm::lookAt( glm::vec3(position),
			  glm::vec3(position) + glm::vec3(dir),
			  glm::vec3(up));
    }

  glm::mat4 CameraComponent::getProjectionMatrix()
  {
      return glm::perspective
	     (glm::radians(this->m_fov),
	      this->m_aspect,
	       this->m_near,
	        this->m_far);
  }

} // namespace scene
} // namespace graphics

#include "engine/scripts/camera_script.h"
#include "graphics/scene/node.h"
#include "graphics/scene/transform.h"
#include "graphics/scene/camera_component.h"
#include "engine/input/input_map.h"
#include <cstdio>
#include <cmath>

namespace engine {
namespace scripts {

void CameraScript::setInputMap(
  engine::input::InputMap* im)
{
  this->m_input = im;
}

void CameraScript::startup()
{
  auto cameras = this->m_owner->getComponents(
			  graphics::scene::component::Type::Camera);

  if(cameras.size() > 0)
  {
    this->m_camera = (graphics::scene::CameraComponent*)cameras[0];
  }
  else
  {
    printf("Warning: A camera script was added to a node without a camera.\n");
  }
}

void CameraScript::update(float dt)
{
  if(this->m_camera == nullptr)
  {
    return;
  }

  if(this->m_input == nullptr)
  {
    return;
  }

  auto owner = this->m_owner;
  auto abs_transform = this->m_owner->getTransform().getAbsoluteMatrix();

  float rotation_speed = 20.0f;
  float move_speed = 10.0f;
  
  float va = this->m_input->getAxisValue(AXIS_VERTICAL) * dt;
  if(std::abs(va) > 0.01)
  {
    glm::vec4 vertical(1,0,0,0);
    vertical = abs_transform * vertical;
    owner->getTransform().translate( move_speed * va * glm::vec3(vertical));
  }

  float ha = this->m_input->getAxisValue(AXIS_HORIZONTAL) * dt;
  if(std::abs(ha) > 0.01)
  {
    glm::vec4 horizontal(0,0,1,0);
    horizontal = abs_transform * horizontal;
    owner->getTransform().translate( move_speed * ha * glm::vec3(horizontal));
  }

  float sa = this->m_input->getAxisValue(AXIS_SKYWARD) * dt;
  if(std::abs(sa) > 0.01)
  {
    glm::vec4 skyward(0,1,0,0);
    skyward = abs_transform * skyward;
    owner->getTransform().translate( move_speed * sa * glm::vec3(skyward));
  }

  float ra = this->m_input->getAxisValue(AXIS_ROTATIONAL) * dt;
  if(std::abs(ra) > 0.01)
  {
    owner->getTransform().rotate(ra * glm::vec3(0, 3.14 / 180 * rotation_speed, 0));
  }
}

void CameraScript::shutdown()
{

}

} // namespace scripts
} // namespace graphics

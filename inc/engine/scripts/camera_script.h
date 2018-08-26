#ifndef ENGINE_CAMERA_SCRIPT_H
#define ENGINE_CAMERA_SCRIPT_H

#include "graphics/scene/script_component.h"
#include <glm/vec3.hpp>

namespace graphics {
namespace scene {
  class CameraComponent;
} // namespace scene
} // namespace graphics

namespace engine {
namespace input {
  class InputMap;
} // namespace input

namespace scripts {

class CameraScript : public graphics::scene::ScriptComponent
{
public:
  void setInputMap(engine::input::InputMap* im);

  virtual void startup() override;
  virtual void update(float dt) override;
  virtual void shutdown() override;

private:
  graphics::scene::CameraComponent* m_camera;
  engine::input::InputMap* m_input;
};

} // namespace scripts
} // namespace engine

#endif // ENGINE_CAMERA_SCRIPT_H

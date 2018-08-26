#ifndef GRAPHICS_SCRIPT_COMPONENT_H
#define GRAPHICS_SCRIPT_COMPONENT_H

#include "graphics/scene/component.h"

namespace graphics {
namespace scene {

class ScriptComponent : public Component
{
public:
  ScriptComponent()
  {
    this->m_type = component::Type::Script;
  }

  virtual void startup() = 0;
  virtual void update(float dt) = 0;
  virtual void shutdown() = 0;
};
  
} // namespace scene
} // namespace graphics

#endif // GRAPHICS_SCRIPT_COMPONENT_H

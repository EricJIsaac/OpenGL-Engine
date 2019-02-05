#include "engine/input/input_map.h"
#include <GLFW/glfw3.h>

namespace engine {
namespace input {

void InputMap::addAxisKey(int key, int axis, float val)
{
  KeyAxisInfo ka;
  ka.axis = axis;
  ka.value = val;
  this->m_keyMapping[key] = ka;
}

void InputMap::removeAxisKey(int key)
{
  this->m_keyMapping.erase(key);
}

void InputMap::key_callback(
    GLFWwindow* window, int key, int scancode, int action, int mods)
{
  for(auto iter = this->m_keyMapping.begin();
    iter != this->m_keyMapping.end(); ++iter )
  {
    if(key == iter->first)
    {
      if(action == GLFW_PRESS)
      {
        KeyAxisInfo ka = iter->second;
        this->m_keyValues[ka.axis] += ka.value;
      }

      if(action == GLFW_RELEASE)
      {
        KeyAxisInfo ka = iter->second;
        this->m_keyValues[ka.axis] = 0.0f;
      }
    }
  }
}

float InputMap::getAxisValue(int axis)
{
  return this->m_keyValues[axis];
}

} // namespace input
} // namespace engine

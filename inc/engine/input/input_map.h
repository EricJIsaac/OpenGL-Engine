#ifndef ENGINE_INPUT_MAP_H
#define ENGINE_INPUT_MAP_H

#include <unordered_map>

struct GLFWwindow;
namespace engine {
namespace input {

#define AXIS_VERTICAL 0
#define AXIS_HORIZONTAL 1
#define AXIS_ROTATIONAL 2
#define AXIS_SKYWARD 3

struct KeyAxisInfo {
public:
  int axis;
  float value;
};

class InputMap
{
public:
  void key_callback(
    GLFWwindow* window, int key, int scancode, int action, int mods);

  void addAxisKey(int key, int axis, float val);
  void removeAxisKey(int key);
  float getAxisValue(int axis);

private:
  std::unordered_map<int, KeyAxisInfo> m_keyMapping;
  std::unordered_map<int, float> m_keyValues;
};

}
} // namespace engine

#endif // ENGINE_INPUT_MAP_H

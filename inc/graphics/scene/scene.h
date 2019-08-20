#ifndef GRAPHICS_SCENE_H
#define GRAPHICS_SCENE_H

#include <string>
#include <memory>
#include <vector>

namespace graphics {
namespace scene {

struct Node;
class Component;

class Scene {
public:
  Scene();
  bool load(std::string fileName);

  void startup();
  void update(float dt);
  void shutdown();

  Node* getRoot();
private:
  void startup(Node* root);
  void update(Node* root, float dt);
  void updateAnimTime(Node* root, float dt);
  void shutdown(Node* root);

  std::unique_ptr<Node> m_rootNode;
  std::vector<std::unique_ptr<Node>> m_nodes;
  std::vector<std::unique_ptr<Component>> m_components;
};

} // namespace scene
} // namespace graphics

#endif // GRAPHICS_SCENE_H

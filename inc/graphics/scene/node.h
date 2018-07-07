#ifndef GRAPHICS_NODE_H
#define GRAPHICS_NODE_H

#include "graphics/scene/transform.h"
#include "graphics/scene/component.h"
#include <vector>

namespace graphics {
namespace scene {
  
struct Node;
class Component;

namespace node {
  void update_child_transforms(Node* root);
  void update_parent_matrices(Node* root, glm::mat4 start);
} // namespace node
  
struct Node
{
public:
  Transform& getTransform();

  void addChild(Node* child);
  void addComponent(Component* component);
  
  const std::size_t numChildren() const;
  const std::size_t numComponents() const;

  Node* getChild(std::size_t index) const;
  std::vector<Component*> getComponents
  (component::Type type);
  
private:
  Transform m_transform;
  std::vector<Node*> m_children;
  std::vector<Component*> m_components;
};

} // namespace scene
} // namespace graphics

#endif // GRAPHICS_NODE_H

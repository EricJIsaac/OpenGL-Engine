#include "graphics/scene/node.h"
#include "graphics/scene/component.h"

namespace graphics {
namespace scene {
namespace node {
  void update_child_transforms(Node* root)
  {
    if(root == nullptr) return;

    update_parent_matrices(root, root->getTransform().getParentMatrix());
  }

  void update_parent_matrices(Node* root, glm::mat4 start)
  {
    if(root == nullptr) return;

    root->getTransform().setParentMatrix(start);
    glm::mat4 new_start =
      root->getTransform().getParentMatrix() *
      root->getTransform().getMatrix();

    for(std::size_t i = 0; i < root->numChildren(); ++i)
      update_parent_matrices(root->getChild(i), new_start);
  }
} // namespace node


  Transform& Node::getTransform()
  {
    return this->m_transform;
  }

  void Node::addChild(Node* child)
  {
    if(child == nullptr) return;

    this->m_children.push_back(child);
  }

  void Node::addComponent(Component* component)
  {
    if(component == nullptr) return;

    this->m_components.push_back(component);
    component->setOwner(this);
  }

  const std::size_t Node::numChildren() const
  {
    return this->m_children.size();
  }

  const std::size_t Node::numComponents() const
  {
    return this->m_components.size();
  }

  Node* Node::getChild(std::size_t index) const
  {
    if(index < 0 || index >= this->numChildren())
      return nullptr;

    return this->m_children[index];
  }

  Component* Node::getComponent(std::size_t index) const
  {
    if(index < 0 || index >= this->numComponents())
      return nullptr;

    return this->m_components[index];
  }

  std::vector<Component*> Node::getComponents(component::Type type)
  {
    std::vector<Component*> out;
    for(std::size_t i = 0; i < this->numComponents(); ++i)
    {
    	if(this->m_components[i]->getType() == type)
    	{
    	  out.push_back(this->m_components[i]);
    	}
    }

    return out;
  }

} // namespace scene
} // namespace graphics

#ifndef GRAPHICS_COMPONENT_H
#define GRAPHICS_COMPONENT_H

namespace graphics {
namespace scene {
namespace component {
enum Type
{
 Camera = 0,
 Mesh,
 SkeletonMesh,
 Script
};
} // namespace component

class Node;
 
class Component
{
public:
  const component::Type getType() const
  { return m_type; }
  
  const Node* getOwner() const
  { return m_owner; }

  const void setOwner(Node* owner)
  { this->m_owner = owner; }
  
protected:
  component::Type m_type;
  Node* m_owner;
};
  
} // namespace scene
} // namespace graphics

#endif // GRAPHICS_COMPONENT_H

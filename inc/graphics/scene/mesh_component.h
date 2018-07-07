#ifndef GRAPHICS_MESH_COMPONENT_H
#define GRAPHICS_MESH_COMPONENT_H

#include "graphics/scene/component.h"

namespace graphics {
namespace scene {

class MeshComponent : public Component
{
public:
  MeshComponent()
  {
    this->m_type = component::Type::Mesh;
  }

  void setId(unsigned int id)
  { this->m_meshId = id; }

  const unsigned int getId() const
  { return this->m_meshId; }

private:
  unsigned int m_meshId;
};

} // namespace scene
} // namespace graphics

#endif // GRAPHICS_MESH_COMPONENT_H

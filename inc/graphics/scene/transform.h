#ifndef GRAPHICS_TRANSFORM_H
#define GRAPHICS_TRANSFORM_H

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace graphics {
namespace scene {

struct Transform
{
public:
  Transform();
  
  void translate(const glm::vec3& translation);
  void rotate(const glm::vec3& rotation);
  void scale(const glm::vec3& scale);

  void setTranslation(const glm::vec3& translation);
  void setRotation(const glm::vec3& rotation);
  void setScale(const glm::vec3& scale);
  void setParentMatrix(glm::mat4 parent);
  
  const glm::vec3 getTranslation() const;
  const glm::vec3 getRotation() const;
  const glm::vec3 getScale() const;

  const glm::mat4 getMatrix() const;
  const glm::mat4 getParentMatrix() const;
  const glm::mat4 getAbsoluteMatrix() const;
  
private:
  glm::vec3 m_translation;
  glm::vec3 m_rotation;
  glm::vec3 m_scale;
  glm::mat4 m_parentMatrix;
};

} // namespace scene
} // namespace graphics

#endif // GRAPHICS_TRANSFORM_H
